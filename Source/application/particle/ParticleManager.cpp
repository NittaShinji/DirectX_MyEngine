#include "ParticleManager.h"
#include "ImGuiManager.h"
#include <d3dcompiler.h>
#include <string.h>
#include <cassert>
#pragma warning(push)
#pragma warning(disable:4061)
#pragma warning(disable:4820)
#include <DirectXTex.h>
#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//静的メンバ変数の実態
ID3D12GraphicsCommandList* ParticleManager::cmdList_;
ID3D12Device* ParticleManager::device_ = nullptr;

ParticleManager::ParticleManager() {}
ParticleManager::~ParticleManager() {}

void ParticleManager::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdlist)
{
	device_ = device;
	cmdList_ = cmdlist;
}

void ParticleManager::Initialize()
{
	//エミッターの初期化
	for(auto& emitter : emitters_)
	{
		emitter->Initialize(device_);
	}
}

void ParticleManager::AllRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->ParticleRemove();
	}

	//エミッターの削除
	emitters_.clear();
}

void ParticleManager::ParticleRemove()
{
	//パーティクルを全削除
	for(auto& emitter : emitters_)
	{
		emitter->ParticleRemove();
	}
}

void ParticleManager::EmitterRemove()
{
	//エミッターの全削除
	emitters_.clear();
}

void ParticleManager::Update(Camera* camera, Attribute attribute)
{
	for(auto& emitter : emitters_)
	{
		emitter->Update(camera,attribute);
	}
}

void ParticleManager::Draw()
{
	for(auto& emitter : emitters_)
	{
		emitter->PreDraw(cmdList_);
		emitter->Draw();
	}
}

//std::unique_ptr<ParticleManager> ParticleManager::Create()
//{
//	//インスタンスを生成
//	std::unique_ptr<ParticleManager> instance = nullptr;
//	instance = std::make_unique<ParticleManager>();
//
//	if(instance == nullptr)
//	{
//		return nullptr;
//	}
//
//	instance->Initialize();
//
//	return instance;
//}

//void ParticleManager::AddEmitter(std::unique_ptr<ParticleEmitter> particleEmitter)
//{
//	emitters_.push_back(std::move(particleEmitter));
//}

void ParticleManager::AddEmitter(ParticleEmitter* particleEmitter)
{
	emitters_.push_back(particleEmitter);
}



//
//void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
//{
//	// コマンドリストをセット
//	//ParticleManager::cmdList_ = cmdList;
//
//	////パイプラインのセット
//	//cmdList_->SetPipelineState(pipelineState_.Get());
//	////ルートシグネチャのセット
//	//cmdList_->SetGraphicsRootSignature(rootSignature_.Get());
//	////プリミティブトポロジーのセット
//	//cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
//}
//
//void ParticleManager::PostDraw()
//{
//	// コマンドリストを解除
//	//ParticleManager::cmdList_ = nullptr;
//}
//
//void ParticleManager::LoadTexture(const std::string& fileName)
//{
//	//const std::string directoryPath = "Resources/Sprite/";
//
//	////ファイルパスを結合
//	//std::string filePath = directoryPath + fileName;
//
//	////ユニコード文字列に変換する
//	//wchar_t wfilePath[128];
//	//MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilePath, _countof(wfilePath));
//
//	////画像ファイルの用意
//	//TexMetadata metadata{};
//	//ScratchImage scratchImg{};
//
//	//HRESULT result = LoadFromWICFile(
//	//	wfilePath, WIC_FLAGS_NONE,
//	//	&metadata, scratchImg);
//
//	//ScratchImage mipChain{};
//	////ミニマップ生成
//	//result = GenerateMipMaps(
//	//	scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
//	//	TEX_FILTER_DEFAULT, 0, mipChain);
//	//if(SUCCEEDED(result))
//	//{
//	//	scratchImg = std::move(mipChain);
//	//	metadata = scratchImg.GetMetadata();
//	//}
//
//	////読み込んだディフューズテクスチャをSRGBとして扱う
//	//metadata.format = MakeSRGB(metadata.format);
//
//	////ヒープ設定
//	//D3D12_HEAP_PROPERTIES textureHeapProp{};
//	//textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	//textureHeapProp.CPUPageProperty =
//	//	D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	//textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//
//	////リソース設定
//	//D3D12_RESOURCE_DESC textureResourceDesc{};
//	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	//textureResourceDesc.Format = metadata.format;
//	//textureResourceDesc.Width = metadata.width; // 幅
//	//textureResourceDesc.Height = (UINT)metadata.height; // 幅
//	//textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	//textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	//textureResourceDesc.SampleDesc.Count = 1;
//
//	////テクスチャバッファの生成
//	//result = device_->CreateCommittedResource(
//	//	&textureHeapProp,
//	//	D3D12_HEAP_FLAG_NONE,
//	//	&textureResourceDesc,
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	nullptr,
//	//	//IID_PPV_ARGS(&textureBuffers_[0]));
//	//	IID_PPV_ARGS(&texbuff_));
//
//	//textureHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//
//	////全ミニマップについて
//	//for(size_t i = 0; i < metadata.mipLevels; i++)
//	//{
//	//	//ミニマップレベルを指定してイメージを取得
//	//	const Image* img = scratchImg.GetImage(i, 0, 0);
//
//	//	//テクスチャバッファにデータ転送
//
//	//	result = texbuff_->WriteToSubresource(
//
//	//		(UINT)i,
//	//		nullptr,
//	//		img->pixels,
//	//		(UINT)img->rowPitch,
//	//		(UINT)img->slicePitch
//	//	);
//
//	//	assert(SUCCEEDED(result));
//	//}
//
//	////デスクリプタヒープの設定
//	//D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
//	//srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	//srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
//	//srvHeapDesc.NumDescriptors = kMaxSRVCount_;
//
//	////設定を本にSRV用デスクリプタヒープを生成
//	//result = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&descHeap_));
//	//assert(SUCCEEDED(result));
//
//	////SRVヒープの先頭ハンドルを取得
//	//sSrvHandle_ = descHeap_->GetCPUDescriptorHandleForHeapStart();
//
//	////シェーダーリソースビューの設定
//	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
//	//srvDesc.Format = textureResourceDesc.Format;//RGBA float
//	//srvDesc.Shader4ComponentMapping =
//	//	D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
//	//srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
//
//	////ハンドルの指す位置にシェーダーリソースビュー作成
//	//device_->CreateShaderResourceView(texbuff_.Get(), &srvDesc, sSrvHandle_);
//
//}


