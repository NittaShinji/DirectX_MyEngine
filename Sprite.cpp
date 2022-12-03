#include "Sprite.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();

	// 頂点データ

	//頂点データ
	vertices.at(0) = {
		{ -0.5f, -0.5f, 0.0f },//左下
	};
	vertices.at(1) = {
		{ -0.5f, +0.5f, 0.0f },//左上
	};
	vertices.at(2) = {
		{ +0.5f, -0.5f, 0.0f },//右下
	};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPos) * vertices.size());

	//カラー
	spriteCommon_->GetConstBufferDataMaterial()->color = XMFLOAT4(0, 1, 0, 0.5f);

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	result_ = directXBasic_->GetResult();
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	//Vertex* vertMap = nullptr;
	VertexPos* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

#pragma region 頂点バッファビューの作成(P02_01)
	
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	//vbView.StrideInBytes = sizeof(vertices[0]);
	vbView.StrideInBytes = sizeof(XMFLOAT3);

#pragma endregion
}

void Sprite::Update()
{
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビューの設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, spriteCommon_->GetConstBuffMaterial()->GetGPUVirtualAddress());

	//描画コマンド(頂点数、インスタンスの数、最初の頂点のインデックス,データを読み取る前に各インデックスに追加される値)
	directXBasic_->GetCommandList()->DrawInstanced(vertices.size(), 1, 0, 0);
}
