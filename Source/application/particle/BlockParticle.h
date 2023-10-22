#pragma once
#include "ObjParticleEmitter.h"

class BlockParticle : public ObjParticleEmitter
{
public:

	static std::unique_ptr<BlockParticle> Create();

	void Initialize();

	void Update(Camera* camera) override;

	void PopUpdate(Camera* camera,const Vector3& popPos);

	void Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, const Vector4& colorSpeed, const Vector3& start_scale, const Vector3& end_scale);

private:

	const std::string modelFileName_;

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向

	Vector3 startScale_ = {};	//初期スケール
	Vector3 endScale_ = {};		//終期スケール

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };

	int32_t particleCount_;

};

