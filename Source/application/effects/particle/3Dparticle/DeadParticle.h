#pragma once
#include "ObjParticleEmitter.h"
#include "GameSpeed.h"

/// <summary>
/// プレイヤーが死んだ時のパーティクル
/// </summary>
class DeadParticle : public ObjParticleEmitter
{
public:

	//インスタンスを生成
	static std::unique_ptr<DeadParticle> Create(std::string modelName);

	//初期化
	void Initialize();

	//更新処理
	void Update(Camera* camera) override;

	//リセット
	void Reset();

	//パーティクル生成準備
	void PopUpdate(Camera* camera, const Vector3& popPos, bool isPlayerDead, Attribute attributeColor);

	//死亡時パーティクル生成
	void DeadParticlePop(Camera* camera, const Vector3& popPos, Attribute attributeColor);

private:

	//パーティクル最大数
	const int32_t kMaxParticleNum_ = 2;

	//座標
	Vector3 setPos_;
	//方向
	Vector3 setVel_;

	//初期スケール
	Vector3 startScale_ = {};
	//終期スケール
	Vector3 endScale_ = {};

	//Imgui設定
	const float kInitImguiValue_ = 0.0f;
	float imGuiPos_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiVel_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiAcc_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };

	//パーティクルを数えるカウンター
	int32_t particleCount_;

	//生成開始フラグ
	bool isStartPoped_;

	//リセットしてもよいか
	bool canReset_;

	//フレーム増加量
	const float freamIncreaseValue_ = 1.0f;

public: //アクセッサ

	//リセットしても良いかどうかのフラグを取得する
	bool GetCanReset() { return canReset_; }
	//リセットしても良いかどうかのフラグをセットする
	void SetCanReset(bool canReset) { canReset_ = canReset; }
};



