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
		float frame = 0;
		//終了フレーム
		float num_frame = 0;

		//色情報
		Vector4 colorSpeed = {};

		//生み出されたどうか
		bool isGenerated;

		//リセットできるかどうか
		bool canReset;
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

	virtual void Preparation();

	void ParticleReset(Camera* camera);

	void Add(const std::string modelName,float life, const Vector3& position, const Vector3& velocity, const Vector3& accel, const Vector4& colorSpeed, const Vector3& start_scale, const Vector3& end_scale);

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
	
	//初期値
	 float InitLife = 30.0f;
	 Vector3 InitPos = { 0.0f,-100.0f,0.0f };
	 Vector3 InitVel = { 0.0f,0.0f,0.0f };

	 Vector3 InitStartScale = { 0.0f,0.0f,0.0f };	//初期化初期スケール
	 Vector3 InitEndScale = { 0.0f,0.0f,0.0f };	//初期化終期スケール

	 Vector3 InitAcc = { 0.0f,0.0f,0.0f };

protected:

	//パーティクル配列
	std::forward_list<Particle> particles_;

	
	//int32_t kMaxParticleNum_ = 3;
	int32_t maxParticleNum_;

	//最大限生成しているかどうか
	bool isMaxParticle_;

	//一度に生成する数
	int32_t generationNum_;

	//現在のパーティクルの数
	int32_t nowParticleCount_;

	//リセットカウント
	int32_t resetCount_;

	//リセットできるか
	bool canReset_;

	std::string modelName_;

};

