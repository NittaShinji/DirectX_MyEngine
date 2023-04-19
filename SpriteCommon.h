#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <array>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include "DirectXBasic.h"
using namespace DirectX;

class SpriteCommon
{
public:
	
	//初期化
	void Initialize(DirectXBasic* directXBasic);
	//更新
	void Update();

	//頂点レイアウト設定
	void VertexLayoutSet();
	//シェーダーの読み込み
	void ShaderLoad();
	//パイプライン設定
	void PipelineSet();
	//ルートシグネチャ設定
	void RootSignatureSet();
	//デスクリプタヒープの設定
	void DescriptorHeapSet();

	//描画開始前
	void BeforeDraw();
	//描画終了後
	void AfterDraw();

	//半透明合成
	void SemiTransparent();
	//加算合成
	void Add();
	//減算合成
	void Sub();
	//色反転
	void InvertColor();
	
	//画像読み込み
	void LoadTexture(uint32_t textureIndex_,const std::string& fileName);

	//描画用テクスチャコマンドの発行
	void SetTextureCommands(uint32_t textureIndex_);

private:

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//色(RGBA)
	};
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;	//色(RGBA)
	};


	//頂点の数
	//static const int layoutCount = 6;

	// 頂点レイアウト
	//std::array<D3D12_INPUT_ELEMENT_DESC,layoutCount> inputLayout{};

	DirectXBasic* directXBasic_ = nullptr;

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	
	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//色用の定数バッファ
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ID3D12Resource* constBuffMaterial = nullptr;

	ConstBufferDataMaterial* constMapMaterial = nullptr;
	//座標用の定数バッファ
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファのGPUリソースのポインタ
	ID3D12Resource* constBuffTransform = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform = nullptr;

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//横方向ピクセル数
	const size_t textureWidth = 256;
	//縦方向ピクセル数
	const size_t textureHeight = 256;
	//配列の要素数
	const size_t imageDateCount = textureWidth * textureHeight;
	//画像イメージデータ配列
	XMFLOAT4* imageDate;

	//シェーダーリソースビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体

	//シェーダーリソース用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	//テクスチャバッファ
	static std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	//テクスチャメモリ用番号
	uint32_t textureHandleIndex_;

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;


public:

	//ゲッター
	DirectXBasic* GetDirectXBasic() { return directXBasic_; };
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffMaterial() { return constBuffMaterial; };
	ConstBufferDataMaterial* GetConstMapMaterial() { return constMapMaterial; };
	ID3D12Resource* GetConstBuffTransform() { return constBuffTransform; };
	ConstBufferDataTransform* GetConstMapTransform() { return constMapTransform; };
	ID3D12DescriptorHeap*  GetSRVHeap() { return srvHeap_.Get(); };
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGpuHandle() { return srvGpuHandle; }

	//定数バッファの生成
	//void CreateConstantBuffer(ID3D12Resource* constBuff);

	//セッター
	//void SetSRVheap(ID3D12DescriptorHeap* srvHeap) { srvHeap_ = srvHeap; };

};