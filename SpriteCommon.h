#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
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
	
	//パイプライン設定
	void PipelineSet();
	//ルートシグネチャ設定
	void RootSignatureSet();

	//半透明合成
	void SemiTransparent();
	//加算合成
	void Add();
	//減算合成
	void Sub();
	//色反転
	void InvertColor();

private:

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//色(RGBA)
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
	//ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

public:

	//ゲッター
	DirectXBasic* GetDirectXBasic() { return directXBasic_; };
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffMaterial() { return constBuffMaterial; };
	ConstBufferDataMaterial* GetConstBufferDataMaterial() { return constMapMaterial; };

};