#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <map>
#include <string>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include "DirectXBasic.h"
#include "MathUtillity.h"

using namespace DirectX;
 
class SpriteCommon final
{
private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//色(RGBA)
	};
	struct ConstBufferDataTransform
	{
		Matrix4 mat;
	};

public:

	//初期化
	void Initialize(DirectXBasic* directXBasic);
	//更新
	void Update();

	static SpriteCommon* GetInstance()
	{
		static SpriteCommon spriteCommon;
		return &spriteCommon;
	}

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
	static void LoadTexture(const std::string& fileName);

	//テンプレートコンストラクタ
	template <typename Type1, typename Type2>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1*& constMapData, Type2* directXBasic_);

private:

	static DirectXBasic* directXBasic_;

	ID3DBlob* vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob_ = nullptr; // エラーオブジェクト

	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	
	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_{};

	//色用の定数バッファ
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//ID3D12Resource* constBuffMaterial = nullptr;
	//座標用の定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//定数バッファのGPUリソースのポインタ
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	//シェーダーリソース用のデスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;

	//テクスチャバッファ
	static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	//テクスチャ番号
	static uint32_t sTextureIndex_;
	//画像に結び付いたテクスチャ番号格納用map
	static std::map<const std::string, uint32_t, std::less<>> textureMap_;

private:

	SpriteCommon();
	~SpriteCommon();

public:

	//コピーコンストラクタを無効
	SpriteCommon(const SpriteCommon& spritecommon) = delete;
	//代入演算子を無効
	SpriteCommon& operator= (const SpriteCommon& spritecommon) = delete;

public:

	//ゲッター
	DirectXBasic* GetDirectXBasic() const { return directXBasic_; };
	ComPtr<ID3D12Resource> GetConstBuffMaterial() const { return constBuffMaterial_; };
	ConstBufferDataMaterial* GetConstMapMaterial() const { return constMapMaterial_; };
	ComPtr<ID3D12Resource> GetConstBuffTransform() const { return constBuffTransform_; };
	ConstBufferDataTransform* GetConstMapTransform() const { return constMapTransform_; };
	ID3D12DescriptorHeap* GetSRVHeap() const { return srvHeap_.Get(); };
	const std::map<const std::string, uint32_t, std::less<>>& GetTextureMap() const { return textureMap_; }
	ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState_; };
	ComPtr<ID3D12RootSignature> GetRootSignature_() { return rootSignature_; };


};