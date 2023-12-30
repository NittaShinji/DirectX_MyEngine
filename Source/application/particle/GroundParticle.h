#pragma once
#include "ParticleManager.h"
#include "Easing.h"

/// <summary>
/// プレイヤーが地面を走っているときのパーティクル
/// </summary>
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
	static const int32_t kChangeColorTime = 30;
	int32_t changeColorTimer_;

	//イージング設定初期値
	const float kInitStartPos = 1.0f;
	const float kInitDistance = -1.0f;
	const float kInitTime = 0.0f;
	const float kInitTotalTime = 20.0f;
	//グラデーション演出用のイージング設定
	EasingInfo redEasing_ = { kInitStartPos,kInitDistance,kInitTime,kInitTotalTime };
	EasingInfo greenEasing_ = { kInitStartPos,kInitDistance,kInitTime,kInitTotalTime };
	EasingInfo blueEasing_ = { kInitStartPos,kInitDistance,kInitTime,kInitTotalTime };

	//プレイヤーの色
	Attribute playerColor_;
	//プレイヤーが加速しているかどうか
	bool isPlayerAxcelled_;

	//黒いパーティクルになっている時間
	static const int32_t kBlackTime_ = 10;
	int32_t blackTime_ = kBlackTime_;

	//パーティクルの色の変更料
	Vector4 colorChangeValue_ = {0.0f,0.0f,0.0f,0.0f};

	//色を変化させる値
	const Vector4 colorSpeed_ = { 0.025f,0.025f,0.025f,0.15f };

	//Imgui設定
	const float kInitImguiValue = 0.0f;
	float imGuiPos_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiVel_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiAcc_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiColor_[4]{ kInitImguiValue,kInitImguiValue,kInitImguiValue,kInitImguiValue };

public: // アクセッサ

	//プレイヤーの色をセット
	void SetIsPlayerColor(Attribute playerColor);
	//プレイヤーが加速しているかどうかをセット
	void SetIsPlayerAxcelled(bool isPlayerAxcelled) { isPlayerAxcelled_ = isPlayerAxcelled; };
};

