#pragma once
#include "ParticleManager.h"
#include "Easing.h"

class GroundParticle : public ParticleEmitter
{
public:

	static std::unique_ptr<GroundParticle> Create(std::string fileName);

	void Preparation(Vector3 playerPos, Attribute playerColor,bool isPlayerAxcelled);

	void ImguiUpdate();

	void Update(Camera* camera) override;
	void SetIsPlayerAxcelled(bool isPlayerAxcelled) { isPlayerAxcelled_ = isPlayerAxcelled; };

private:

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向
	
	float startScale_ = 1.0f;	//初期スケール
	float endScale_ = 4.0f;		//終期スケール

	bool isPlayerAxcelled_;
	Vector4 endColor_;
	Vector4 startColor_;

	const float endColorSpeed_ = 0.001f;
	static const int32_t kChangeColorTime = 30;
	int32_t changeColorTimer_;
	bool isFinishedWhite_;
	EasingInfo whiteEasing_ = { 1.0f,-1.0f,0.0f,20.0f };

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };
	float imGuiColor_[4]{ 0.0f,0.0f,0.0f,0.0f};
	//float imGuiColor[3]{ 0.0f,0.0f,0.0f};

};

