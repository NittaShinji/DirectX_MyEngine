#pragma once
#include "Object3D.h"
#include "Camera.h"
#include <forward_list>
#include <vector>

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager 
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	
	//頂点情報
	struct Vertex
	{
		Vector3 pos;		// xyz座標
		float scale;		// 大きさ
		XMFLOAT4 color;		// 色
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX viewProjection;	// ３Ｄ変換行列
	};

	struct Particle
	{
		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};

		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;

		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;

		XMFLOAT4 color = { 1,1,1,1 };

		XMFLOAT4 colorSpeed = { 0.01f,0.0f,0.0f,1.0f };

	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = 1024;		// 頂点数
	//static const int indexCount = 3*2;		// インデックス数


public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device);

private: // 静的メンバ変数

	//ビルボード行列
	//static XMMATRIX matBillboard;
	//Y軸周りビルボード行列
	//static XMMATRIX matBillboardY;

private:// 静的メンバ関数


public: // メンバ関数

	void Initialize();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	void InitializeGraphicsPipeline();

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	static std::unique_ptr<ParticleManager> Create();

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();


	void Add(int life,Vector3 position, Vector3 velocity, Vector3 accel, XMFLOAT4 colorSpeed,float start_scale, float end_scale);

	//static void LoadTexture();
	static void LoadTexture(const std::string& fileName);


	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);

	bool GetIsMaxParticle() { return isMaxParticle_; }

	void SetScale();

	std::forward_list<Particle> GetPaticles() { return particles_; }

	void SetGenerationNum(int32_t generationNum) { generationNum_ = generationNum; }

private: // メンバ変数

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff_; 
	//定数バッファのマッピング用ポインタ
	ConstBufferData* constMapData_ = nullptr;

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	static ComPtr<ID3D12PipelineState> pipelineState_;

	//ジオメトリシェーダーオブジェクト
	
	// ビュー行列
	XMMATRIX matView_;
	// 射影行列
	XMMATRIX matProjection_;

	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff_;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff_;
	//頂点データ配列
	static std::vector<Vertex> vertices_;
	//static std::forward_list<Vertex> vertices_;


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

	static ID3D12GraphicsCommandList* cmdList_;
	static ID3D12Device* device_;

	DirectXBasic* directXBasic_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};

	//パーティクル配列
	//std::forward_list<Particle> particles_;
	std::forward_list<Particle> particles_;

	//最大限生成しているかどうか
	bool isMaxParticle_;

	//生成済みのパーティクルの数
	int32_t nowParticleCount_ = 0;

	bool isFinish_ = 0;

	//一度に生成する数
	int32_t generationNum_;

};

