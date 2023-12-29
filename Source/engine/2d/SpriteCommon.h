#pragma once
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <map>
#include <string>
#include "Vector4.h"
#include "DirectXBasic.h"
#include "MathUtillity.h"
#include "TextureManager.h"

class SpriteCommon final
{
private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		Vector4 color;	//色(RGBA)
	};
	struct ConstBufferDataTransform
	{
		Matrix4 mat;
	};

public:

	//初期化
	void Initialize(DirectXBasic* directXBasic);

	//インスタンスを取得
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

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

private:

	//コンストラクタ
	SpriteCommon();
	//デストラクタ
	~SpriteCommon();

public:

	//コピーコンストラクタを無効
	SpriteCommon(const SpriteCommon& spritecommon) = delete;
	//代入演算子を無効
	SpriteCommon& operator= (const SpriteCommon& spritecommon) = delete;

public:

	//ゲッター
	//DirextXBasicのポインタを渡す
	DirectXBasic* GetDirectXBasic() const { return directXBasic_; };
	//パイプラインステートを取得
	ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState_; };
	//ルートシグネチャを取得
	ComPtr<ID3D12RootSignature> GetRootSignature_() { return rootSignature_; };
};