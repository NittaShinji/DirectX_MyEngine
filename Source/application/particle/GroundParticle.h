#pragma once
#include "ParticleManager.h"

class GroundParticle : public ParticleEmitter
{
public:

	static std::unique_ptr<GroundParticle> Create(std::string fileName);

	void Preparation(Vector3 playerPos, Attribute playerColor);

	void ImguiUpdate();

private:

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向
	
	float startScale_ = 1.0f;	//初期スケール
	float endScale_ = 4.0f;		//終期スケール

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };
	float imGuiColor_[4]{ 0.0f,0.0f,0.0f,0.0f};
	//float imGuiColor[3]{ 0.0f,0.0f,0.0f};

};

