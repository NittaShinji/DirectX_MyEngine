#pragma once
#include "Sprite.h"
#include "Easing.h"

/// <summary>
/// 結果画面に表示する画像をまとめて描画するクラス
/// </summary>
class ResultSprite
{
public:

	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//画面外に出る
	void ComeOutOffScreen();

	//画面内に来る
	void ComeInScreen();

	//リセット
	void Reset();

private: //定数

	const float kEasingTime_ = 60.0f;

private: //メンバ変数

	EasingInfo resultOutEasing_ = { 0.0f, 0.0f, 0.0f, kEasingTime_ };
	EasingInfo resultInEasing_ = { 0.0f, 0.0f, 0.0f, kEasingTime_ };

	bool isFinishOutEasing_ = true;
	bool isFinishInEasing_ = false;

	//背景スプライト
	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;
	//Aボタンスプライト
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	//ステージスプライト
	std::unique_ptr<Sprite> stageSprite_ = nullptr;
	//ネクストスプライト
	std::unique_ptr<Sprite> nextSprite_ = nullptr;

public: //アクセッサ

	float GetBackGroundSpritePosY() { return backGroundSprite_.get()->GetPosition().y; }
	bool GetIsFinishOutEasing() { return isFinishOutEasing_; }
	bool GetIsFinishInEasing() { return isFinishInEasing_; }

	void SetIsFinishOutEasing(bool isFinishOutEasing) { isFinishOutEasing_ = isFinishOutEasing; }
	void SetIsFinishInEasing(bool isFinishInEasing) { isFinishInEasing_ = isFinishInEasing; }
};

