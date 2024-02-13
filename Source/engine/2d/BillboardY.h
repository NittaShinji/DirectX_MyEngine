#pragma once
#include "Camera.h"
#include "Vector4.h"
#include "ObjectAttribute.h"
#include "GameSpeed.h"
#include <forward_list>

/// <summary>
/// Y軸ビルボール
/// </summary>

class BillboardY
{

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス

	//頂点情報
	struct Vertex
	{
		Vector3 pos;		// xyz座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 viewProjection;	// ３Ｄ変換行列
		Matrix4 matBillboard;	// ビルボード行列
	};

	struct BillboardYSprite
	{
		//座標
		Vector3 position = {};
		//回転
		float rotation = 0.0f;
		//スケール
		float scale = 1.0f;
	};

	enum class BillboardType
	{
		AllDirection,
		Yaxis,
	};

public: // メンバ関数

	//静的インスタンスを生成
	static std::unique_ptr<BillboardY> Create(std::string fileName);

	//初期化
	virtual void Initialize(BillboardType billboardType_);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	static void InitializeDescriptorHeap();

	//描画前処理
	static void PreDraw();

	//描画後処理
	static void PostDraw();

	//静的初期化
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdlist);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//板ポリモデルを生成
	void CreateModel();

	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);

private: // 定数

	static const int vertexCount = 1;		// 頂点数

private:

	//定数バッファのマッピング用ポインタ
	ConstBufferData* constMapData_ = nullptr;

	//グラフィックスパイプライン
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_;
	//パープラインステート
	static ComPtr<ID3D12PipelineState> pipelineState_;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;

	//頂点データ配列
	static Vertex vertices_[vertexCount];

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap_;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize_;

	//デスクリプタヒープハンドル
	static D3D12_CPU_DESCRIPTOR_HANDLE sSrvHandle_;
	//テクスチャ番号
	static uint32_t sTextureIndex_;

	// シェーダリソースビューのハンドル(CPU)
	static D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(CPU)
	static D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature_;
	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;
	static const UINT kRenderTexNum = 2;

	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	//デバイス
	static ID3D12Device* device_;
	//DirectXの基礎情報
	DirectXBasic* directXBasic_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};

protected:

	//ジオメトリシェーダーオブジェクト
	// ビュー行列
	Matrix4 matView_;
	// 射影行列
	Matrix4 matProjection_;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;

	//回転角
	float rotation_;

	// 頂点数
	static const int kVertexCount = 1024;

	//表示する画像名
	std::string fileName_;

	//ゲームスピード
	GameSpeed* gameSpeed_ = nullptr;

	//ビルボードの型
	BillboardType billboardType_;

public: //アクセッサ

	//ゲームスピードをセット
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }
};

