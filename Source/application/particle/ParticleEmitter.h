#pragma once
#include "Camera.h"
#include "Vector4.h"
#include "ObjectAttribute.h"
#include "GameSpeed.h"
#include <forward_list>

/// <summary>
/// 3Dオブジェクト
/// </summary>

class ParticleEmitter
{

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス

	//頂点情報
	struct Vertex
	{
		Vector3 pos;		// xyz座標
		float scale;		// 大きさ
		Vector4 color;		// 色
		float rotate;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 viewProjection;	// ３Ｄ変換行列
		Matrix4 matBillboard;	// ビルボード行列
	};

	struct Particle
	{
		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};

		float rotation = 0.0f;
		float rotationSpeed = 0.1f;

		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;

		//現在フレーム
		float frame = 0;
		//終了フレーム
		float num_frame = 0;
		//色情報
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 endColor = {0.0f,0.0f,0.0f,1.0f};
		Vector4 colorSpeed = { 0.0f,0.0f,0.1f,0.0f };
	};

public: // メンバ関数

	virtual void Initialize(ID3D12Device* device);

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
	virtual void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	static std::unique_ptr<ParticleEmitter> Create();

	//頂点バッファの生成
	void CreateVertBuff();

	void Add(float life, Vector3 position, Vector3 velocity, Vector3 accel, Vector4 endColor,Vector4 color,Vector4 colorSpeed, float start_scale, float end_scale,float rotation,float rotationSpeed);

	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);

	bool GetIsMaxParticle() { return isMaxParticle_; }

	void SetScale();
	void SetRotation(float rotation) { rotation_ = rotation; };

	void ParticleRemove();

	std::forward_list<Particle> GetPaticles() { return particles_; }

	void SetGenerationNum(int32_t generationNum) { generationNum_ = generationNum; }
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }

private: // 定数
		static const int division = 50;					// 分割数
		static const float radius;				// 底面の半径
		static const float prizmHeight;			// 柱の高さ
		static const int planeCount = division * 2 + division * 2;		// 面の数
		static const int kVertexCount = 1024;		// 頂点数

private:

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	//定数バッファのマッピング用ポインタ
	ConstBufferData* constMapData_ = nullptr;

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	static ComPtr<ID3D12PipelineState> pipelineState_;

	//ジオメトリシェーダーオブジェクト
	// ビュー行列
	Matrix4 matView_;
	// 射影行列
	Matrix4 matProjection_;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	//頂点データ配列
	std::vector<Vertex> vertices_;

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
	std::forward_list<Particle> particles_;

	//最大限生成しているかどうか
	bool isMaxParticle_;

	bool isFinish_ = 0;

	//一度に生成する数
	int32_t generationNum_;

	//現在のパーティクルの数
	int32_t nowParticleCount_ = 0;

	float rotation_;

protected :

	const float kColorPinkR = 0.965f;
	const float kColorPinkG = 0.122f;
	const float kColorPinkB = 0.325f;
	const float kColorPinkAlpha = 1.0f;

	const float kColorYellowR = 0.957f;
	const float kColorYellowG = 0.596f;
	const float kColorYellowB = 0.333f;
	const float kColorYellowAlpha = 1.0f;

	//フレーム増加量
	const float freamIncreaseValue_ = 1.0f;

	//表示する画像名
	std::string particleFileName_;

	//ゲームスピード
	GameSpeed* gameSpeed_ = nullptr;
};

