#pragma once
#include "Object3d.h"
#include "ObjectAttribute.h"
#include <forward_list>

class ObjParticleEmitter
{
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

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int kVertexCount = 3;		// 頂点数

private:

	//パーティクル配列
	std::forward_list<Particle> particles_;

	//最大限生成しているかどうか
	bool isMaxParticle_;

	//一度に生成する数
	int32_t generationNum_;

	//現在のパーティクルの数
	int32_t nowParticleCount_;

};

