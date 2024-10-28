#pragma once
#include "ParticleEmitter.h"

/// <summary>
/// 衝突時のパーティクル
/// </summary>
class HitParticle2D : public ParticleEmitter
{
public:

	//インスタンスを生成
	static std::unique_ptr<HitParticle2D> Create(std::string fileName);

	//パーティクル生成準備
	void Preparation(Vector3 playerPos, bool isPlayerDead);

	//更新処理
	void Update(Camera* camera) override;

	//Imgui更新処理
	void ImguiUpdate();

private: //定数

	//一度に生成する数
	const int32_t kOneTimeGenerationNum = 3;
	//初期のスケール初期値
	const float kInitiStartScale = 1.0f;
	//終点のスケール初期値
	const float kInitiEndScale = 20.0f;
	//回転角の回転値
	const float kMoveRotateValue_ = 0.01f;

private: //メンバ変数


	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向
	
	float startScale_ = kInitiStartScale;	//初期スケール
	float endScale_ = kInitiEndScale;		//終期スケール

	//生成待ち時間
	static const int32_t kWaitingTime = 20;
	int32_t waitingTimeforGeneration_;
	bool isGenerated_ = true;

	//Imgui設定
	const float kInitImguiValue = 0.0f;
	float imGuiPos_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiVel_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiAcc_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiColor_[4]{ kInitImguiValue,kInitImguiValue,kInitImguiValue,kInitImguiValue };
};
