#pragma once
#include "ParticleManager.h"

class SecondJump2DParticle : public ParticleEmitter
{
public:

	static std::unique_ptr<SecondJump2DParticle> Create(std::string fileName);

	void Preparation(Vector3 playerPos);

	void ImGuiUpdate();

private:

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向

	float startScale_ = 1.0f;	//初期スケール
	float endScale_ = 2.0f;		//終期スケール

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };
};