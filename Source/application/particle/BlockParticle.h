#pragma once
#include "ObjParticleEmitter.h"
#include "ObjectAttribute.h"
#include "GameSpeed.h"

/// <summary>
/// 四角いオブジェクトのパーティクル
/// </summary>
class BlockParticle : public ObjParticleEmitter
{
public:

	//インスタンスを生成
	static std::unique_ptr<BlockParticle> Create(std::string modelName, int32_t maxParticleNum);

	//初期化
	void Initialize();

	//更新
	void Update(Camera* camera) override;

	//リセット
	void Reset();

	//パーティクル生成時の更新処理
	void PopUpdate(Camera* camera,const Vector3& popPos,bool isLanded,bool isPlayerDead,Attribute attributeColor);

	//パーティクル生成
	void ParticlePop(Camera* camera, const Vector3& popPos,Attribute attributeColor);

private:

	//初期スケール初期値
	const Vector3 kInitStartScale_ = { 0.9f,0.9f,0.9f };
	//終期スケール初期値
	const Vector3 kInitEndScale_ = { 0.0f,0.0f,0.0f };

	//初期カウンター数
	const int32_t kInitCount_ = 0;

	//座標
	Vector3 setPos_;
	//方向
	Vector3 setVel_;		

	//初期スケール
	Vector3 startScale_ = {};	
	//終期スケール
	Vector3 endScale_ = {};		

	//Imgui設定
	const float kInitImguiValue = 0.0f;
	float imGuiPos_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiVel_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };
	float imGuiAcc_[3]{ kInitImguiValue,kInitImguiValue,kInitImguiValue };

	int32_t particleCount_;

	//生成開始フラグ
	bool isStartPoped_;

	//ゲームオーバーかどうか
	bool isPlayerDead_;

	//リセットしてもよいか
	bool canReset_;

	//属性色
	Attribute attributeColor_;

	//ゲームスピード
	GameSpeed* gameSpeed_ = nullptr;


public: //アクセッサ

	//リセットしても良いかどうかのフラグを取得する
	bool GetCanReset() { return canReset_; }
	//プレイヤーが死んでいるかどうかをセット
	void SetPlayerIsDead(bool isPlayerDead) { isPlayerDead_ = isPlayerDead; }
	//リセットしても良いかどうかのフラグをセットする
	void SetCanReset(bool canReset) { canReset_ = canReset; }
	//ゲームスピードをセットする
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }
};

