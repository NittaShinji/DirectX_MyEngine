#pragma once
#include "ObjParticleEmitter.h"
#include "GameSpeed.h"

/// <summary>
/// 壁破壊時のパーティクル
/// </summary>
class BreakParticle : public ObjParticleEmitter
{
public:

	enum breakDir
	{
		FrontUp = 0,
		BackUp,
		FrontDown,
		BackDown,
	};

public:

	//インスタンスを生成
	static std::unique_ptr<BreakParticle> Create(std::string modelName);

	//初期化
	void Initialize();

	//更新
	void Update(Camera* camera) override;

	//リセット
	void Reset();

	//パーティクル生成準備
	void Preparation() override;

	//パーティクル生成
	void BreakParticlePop(Camera* camera, const Vector3& popPos);

	//パーティクル生成時の更新処理
	void PopUpdate(Camera* camera, std::vector<Vector3> breakWallsPos);

	//ImGuiの更新処理
	void ImGuiUpdate();

private:

	//パーティクル最大数
	const int32_t kMaxParticleNum_ = 4;

	//座標
	Vector3 setPos_;
	//方向
	Vector3 setVel_;

	//初期スケール初期値
	const Vector3 kInitStartScale_ = { 0.9f,0.9f,0.9f };
	//終期スケール初期値
	const Vector3 kInitEndScale_ = { 0.0f,0.0f,0.0f };

	//初期カウンター数
	const int32_t kInitCount_ = 0;

	//初期スケール
	Vector3 startScale_ = {};
	//終期スケール
	Vector3 endScale_ = {};

	const float kMoveAlphaValue_ = 0.03f;

	//Imgui設定
	const float kInitImguiValue_ = 0.0f;
	float imGuiPos_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiVel_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };
	float imGuiAcc_[3]{ kInitImguiValue_,kInitImguiValue_,kInitImguiValue_ };

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

