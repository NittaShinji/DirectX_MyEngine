#pragma once
#include "ObjParticleEmitter.h"
#include "GameSpeed.h"

class BreakParticle : public ObjParticleEmitter
{
public:

	static std::unique_ptr<BreakParticle> Create(std::string modelName);

	void Initialize();

	void Update(Camera* camera) override;

	void Reset();

	void PopUpdate(Camera* camera, std::vector<Vector3> breakWallsPos);

	void Preparation() override;

	void BreakParticlePop(Camera* camera, const Vector3& popPos);

	bool GetCanReset() { return canReset_; }

	void SetPlayerIsDead(bool isPlayerDead) { isPlayerDead_ = isPlayerDead; }
	void SetCanReset(bool canReset) { canReset_ = canReset; }
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }

	void ImGuiUpdate();

private:

	//パーティクル最大数
	const int32_t kMaxParticleNum_ = 4;

	//座標
	Vector3 setPos_;
	//方向
	Vector3 setVel_;

	//初期スケール
	Vector3 startScale_ = {};
	//終期スケール
	Vector3 endScale_ = {};

	float imGuiPos_[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel_[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc_[3]{ 0.0f,0.0f,0.0f };

	int32_t particleCount_;

	//生成開始フラグ
	bool isStartPoped_;
	//ゲームオーバーかどうか
	bool isPlayerDead_;
	//リセットしてもよいか
	bool canReset_;

	//フレーム増加量
	const float freamIncreaseValue_ = 1.0f;
	//ゲームスピード
	GameSpeed* gameSpeed_ = nullptr;

};

