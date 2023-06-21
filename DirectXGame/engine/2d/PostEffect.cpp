#include "PostEffect.h"
#include "WindowsAPI.h"
#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() 
	: Sprite()
{
}

void PostEffect::Initialize()
{
	HRESULT result;

	//基底クラスとしての初期化
	Sprite::Initialize(XMFLOAT2(0, 0), XMFLOAT2(170, 170));

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowsAPI::kWindow_width_,
		(UINT)WindowsAPI::kWindow_height_,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	D3D12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	//テクスチャバッファの生成
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	//テクスチャを赤クリア
	//画素数(1280 x 720 = 921600ピクセル)
	const UINT pixelCount = WindowsAPI::kWindow_width_ * WindowsAPI::kWindow_height_;
	//画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WindowsAPI::kWindow_width_;
	//画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WindowsAPI::kWindow_height_;
	//画像イメージ
	UINT* img = new UINT[pixelCount];
	for(int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(0, nullptr,
		img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用のデスクリプタヒープを生成
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};									//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;						//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV生成
	directXBasic_->GetDevice()->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);


}

void PostEffect::Draw(const std::string& fileName)
{
	matUpdate();

	//パイプラインステートの設定
	directXBasic_->GetCommandList()->SetPipelineState(spriteCommon_->GetPipelineState().Get());
	//ルートシグネチャの設定
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature_().Get());
	//プリミティブ形状を設定
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	spriteCommon_->Update();
	
	//頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//SRVヒープの設定コマンド
	//ID3D12DescriptorHeap* heaps[] = { spriteCommon_->GetSRVHeap() };
	ID3D12DescriptorHeap* heaps[] = { descHeapSRV.Get()};
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//GPUのSRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteCommon_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	uint32_t textureIndex;
	textureIndex = spriteCommon_->GetTextureMap().at(fileName);

	//取得したサイズを使用してハンドルを進める
	for(uint32_t i = 0; i < textureIndex; i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	//directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, spriteCommon_->GetConstBuffTransform()->GetGPUVirtualAddress());

	if(isInvisible_)
	{
		return;
	}

	//描画コマンド(頂点数、インスタンスの数、最初の頂点のインデックス,データを読み取る前に各インデックスに追加される値)
	directXBasic_->GetCommandList()->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);
}
