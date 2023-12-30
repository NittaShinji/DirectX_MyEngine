#pragma once
#include "ParticleManager.h"

/// <summary>
/// プレイヤーのジャンプ時に出る半透明の円形パーティクル
/// </summary>
class SecondJump2DParticle : public ParticleEmitter
{
public:

	//インスタンスを生成
	static std::unique_ptr<SecondJump2DParticle> Create(std::string fileName);

	//パーティクル生成準備
	void Preparation(Vector3 playerPos);

	//更新
	void Update(Camera* camera) override;

	//ImGui更新
	void ImGuiUpdate();

private:

	Vector3 setPos_;		//座標
	Vector3 setVel_;		//方向

	float startScale_ = 4.5f;	//初期スケール
	float endScale_ = 1.0f;	//終期スケール

	//imGui設定
	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };
};