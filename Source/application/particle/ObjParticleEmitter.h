#pragma once
#include "Object3d.h"
#include "ObjectAttribute.h"
#include <forward_list>

class ObjParticleEmitter
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//struct GSOutput
	//{
	//	Vector3 pos;		// xyz座標
	//	Vector3 normal;	//法線ベクトル
	//	Vector2 uv;		// uv座標
	//};

	//頂点情報
	//struct GSOutput
	//{
	//	Vector3 pos;		// xyz座標
	//	Vector3 normal;	//法線ベクトル
	//	Vector2 uv;		// uv座標
	//};

	// 定数バッファ用データ構造体
	//struct ConstBufferData
	//{
	//	Matrix4 viewProjection;	// ３Ｄ変換行列
	//};

	struct Particle
	{
		Object3d object3d;

		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};

		Vector3 scale = {};

		//初期値
		Vector3 s_scale = {};
		//最終値
		Vector3 e_scale = {};

		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;

		//色情報
		Vector4 colorSpeed = {};
	};

public: // メンバ関数

	virtual void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	static std::unique_ptr<ObjParticleEmitter> Create();

	void Add(const std::string fileName,int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, const Vector4& colorSpeed, const Vector3& start_scale, const Vector3& end_scale);

	bool GetIsMaxParticle() { return isMaxParticle_; }

	void AllRemove();

	std::forward_list<Particle> GetPaticles() { return particles_; }

	void SetGenerationNum(int32_t generationNum) { generationNum_ = generationNum; }

	void SetIsMaxParticle(bool isMaxParticle) { isMaxParticle_ = isMaxParticle; }

	std::size_t GetParticleNum() { return std::distance(particles_.begin(), particles_.end()); }

protected: // 定数

	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int kVertexCount = 5;		// 頂点数

protected:

	//定数バッファ
	//ComPtr<ID3D12Resource> constBuff_;
	////定数バッファのマッピング用ポインタ
	//ConstBufferData* constMapData_ = nullptr;

	////グラフィックスパイプライン
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	//static ComPtr<ID3D12PipelineState> pipelineState_;

	////ジオメトリシェーダーオブジェクト

	//// ビュー行列
	//Matrix4 matView_;
	//// 射影行列
	//Matrix4 matProjection_;

	//// テクスチャバッファ
	//static ComPtr<ID3D12Resource> texbuff_;
	//// 頂点バッファ
	//static ComPtr<ID3D12Resource> vertBuff_;
	////頂点データ配列
	//static std::vector<Vertex> vertices_;

	//// デスクリプタヒープ
	//static ComPtr<ID3D12DescriptorHeap> descHeap_;
	//// デスクリプタサイズ
	//static UINT descriptorHandleIncrementSize_;

	////デスクリプタヒープハンドル
	//static D3D12_CPU_DESCRIPTOR_HANDLE sSrvHandle_;
	////テクスチャ番号
	//static uint32_t sTextureIndex_;

	//// シェーダリソースビューのハンドル(CPU)
	//static D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	//// シェーダリソースビューのハンドル(CPU)
	//static D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

	//// ルートシグネチャ
	//static ComPtr<ID3D12RootSignature> rootSignature_;
	////SRVの最大個数
	//static const size_t kMaxSRVCount_ = 2056;
	//static const UINT kRenderTexNum = 2;

	//static ID3D12GraphicsCommandList* cmdList_;
	//static ID3D12Device* device_;

	//DirectXBasic* directXBasic_ = nullptr;

	//// 頂点バッファビュー
	//D3D12_VERTEX_BUFFER_VIEW vbView_ = {};

	//パーティクル配列
	std::forward_list<Particle> particles_;

	//最大限生成しているかどうか
	bool isMaxParticle_;

	//一度に生成する数
	int32_t generationNum_;

	//現在のパーティクルの数
	int32_t nowParticleCount_;

};

