#pragma once
#include "ObjParticleEmitter.h"
#include "GameSpeed.h"

/// <summary>
/// 着地時のパーティクル
/// </summary>
class LandParticle : public ObjParticleEmitter
{
public:

	//インスタンスを生成
	static std::unique_ptr<LandParticle> Create(std::string modelName);

	//初期化
	void Initialize();

	//更新
	void Update(Camera* camera) override;

	//リセット
	void Reset();

	//パーティクル生成中の更新処理
	void PopUpdate(Camera* camera, const Vector3& popPos, bool isLanded,Attribute attributeColor);

	//パーティクルを出現させる
	void LandParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor);

	//プレイヤーと一定の距離離れたら、パーティクルをリセットする関数
	void ResetParticleArea(Vector3 playerPos);

	bool GetCanReset() { return canReset_; }

	void SetPlayerIsDead(bool isPlayerDead) { isPlayerDead_ = isPlayerDead; }
	void SetCanReset(bool canReset) { canReset_ = canReset; }
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }

private:

	//初期カウンター数
	const int32_t kInitCount = 0;

	//パーティクル最大数
	const int32_t kMaxParticleNum_ = 6;

	//リセットするプレイヤーとの距離
	const float kResetDitance_ = 30.0f;

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

	//パーティクルを数えるカウンター
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

