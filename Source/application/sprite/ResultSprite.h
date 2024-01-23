#pragma once
#include "Sprite.h"
#include "Easing.h"

class ResultSprite
{
public:

	//読み込み処理
	void Load();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();


private: //定数

	const float kEasingTime_ = 60.0f;

private: //メンバ変数

	EasingInfo resultEasing_ = { 0.0f, 0.0f, 0.0f, kEasingTime_ };

	bool isFinishBackGroundEasing_ = false;

	///背景スプライト
	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;

	//イージング移動距離
	float easingMoveDistance_;

public: //アクセッサ

	float GetBackGroundSpritePosY() { return backGroundSprite_.get()->GetPosition().y; }
	bool GetIsFinishBackGroundEasing() { return isFinishBackGroundEasing_; }

};

