#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include"GamePad.h"
#include "Vector2.h"
#include "SoundManager.h"

/// <summary>
/// クリアシーン
/// </summary>
class ClearScene : public BaseScene
{

public:

	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

private:

	std::unique_ptr<Sprite> end_ = nullptr;
	std::unique_ptr<Sprite> check_ = nullptr;
	std::unique_ptr<Sprite> aButton_ = nullptr;
	std::unique_ptr<Sprite> aButtonPushed_ = nullptr;

	std::unique_ptr<Sprite> clearSprite_ = nullptr;

	static DirectXBasic* directXBasic_;
	static ImGuiManager* imGuiManager_;

	static KeyInput* keys_;
	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	Vector2 checkPosition_;

	const int32_t kActionTime_ = 60;
	int32_t moveTimer_ = kActionTime_;
	Vector2 move_;
	float rotate_;

	//1周回る時間
	const float kRotateTime_ = 30.0f;
	float rotateTimer_ = kRotateTime_;
	bool isRotateSprite_ = false;

	//サウンド
	std::unique_ptr<Sound> checkSound_ = nullptr;

	//ボタンが押されたかどうか
	bool isPushedAbutton_;
};

