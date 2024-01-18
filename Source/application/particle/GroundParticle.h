#pragma once
#include "ParticleEmitter.h"
#include "Easing.h"

/// <summary>
/// プレイヤーが地面を走っているときのパーティクル
/// </summary>
class Player;

class GroundParticle : public ParticleEmitter
{
public:

	//インスタンスを生成
	static std::unique_ptr<GroundParticle> Create(std::string fileName);

	//パーティクル生成準備
	void Preparation(Vector3 playerPos, Attribute playerColor,bool isPlayerAxcelled,bool isSlow);

	//ImGui更新
	void ImguiUpdate();

	//更新
	void Update(Camera* camera) override;

private:

	//一度に生成する数
	const int32_t kOneTimeGenerationNum = 3;

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向
	
	float startScale_ = 1.0f;	//初期スケール
	float endScale_ = 4.0f;		//終期スケール

	Vector4 startColor_;		//初期色
	Vector4 endColor_;			//終点色

	//色変化時間
	static const int32_t kChangeColorTime_ = 30;
	int32_t changeColorTimer_;

	//イージング設定初期値
	const float kInitStartPos_ = 1.0f;
	const float kInitDistance_ = -1.0f;
	const float kInitTime_ = 0.0f;
	const float kInitTotalTime_ = 20.0f;
	//グラデーション演出用のイージング設定
	EasingInfo redEasing_ = { kInitStartPos_,kInitDistance_,kInitTime_,kInitTotalTime_ };
	EasingInfo greenEasing_ = { kInitStartPos_,kInitDistance_,kInitTime_,kInitTotalTime_ };
	EasingInfo blueEasing_ = { kInitStartPos_,kInitDistance_,kInitTime_,kInitTotalTime_ };

	//プレイヤーの色
	Attribute playerColor_;

	//黒いパーティクルになっている時間
	static const int32_t kBlackTime_ = 10;
	int32_t blackTime_ = kBlackTime_;

	//パーティクルの色の変更料
	Vector4 colorChangeValue_ = {0.0f,0.0f,0.0f,0.0f};

	//色を変化させる値
	const Vector4 colorSpeed_ = { 0.025f,0.025f,0.025f,0.15f };

	//Imgui設定
	const float kInitImguiValue_ = 0.0f;
	float imGuiPos_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiVel_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiAcc_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiColor_[4]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };

public: //アクセッサ

	void SetIsPlayerColor(Attribute playerColor) { playerColor_ = playerColor; }

};

