#pragma once
#include "ParticleManager.h"

class HitParticle2D : public ParticleEmitter
{
public:

	static std::unique_ptr<HitParticle2D> Create(std::string fileName);

	void Preparation(Vector3 playerPos,bool isPlayerTouchObject);

	void ImguiUpdate();

	void Update(Camera* camera) override;


private:

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向

	float startScale_ = 1.0f;	//初期スケール
	float endScale_ = 20.0f;		//終期スケール

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };
	float imGuiColor_[4]{ 0.0f,0.0f,0.0f,0.0f };

	//生成待ち時間
	static const int32_t kWaitingTime = 20;
	int32_t waitingTimeforGeneration_;
	bool isGenerated_ = true;

public:

	bool GetIsGenerated() { return isGenerated_; }

	//float imGuiColor[3]{ 0.0f,0.0f,0.0f};

};
