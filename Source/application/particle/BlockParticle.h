#pragma once
#include "ObjParticleEmitter.h"

class BlockParticle : public ObjParticleEmitter
{
public:

	static std::unique_ptr<BlockParticle> Create();

	void Initialize();

	void Preparation(const Vector3& popPos, std::string fileName);

	void Update(Camera* camera) override;

private:

	const std::string modelFileName_;

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向

	Vector3 startScale_ = {};	//初期スケール
	Vector3 endScale_ = {};		//終期スケール

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };

};

