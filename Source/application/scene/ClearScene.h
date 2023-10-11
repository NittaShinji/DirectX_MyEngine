#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include"GamePad.h"
#include "Vector2.h"
#include "SoundManager.h"

class ClearScene : public BaseScene
{

public:

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

	static void StaticInitialize();

private:

	std::unique_ptr<Sprite> end_ = nullptr;
	std::unique_ptr<Sprite> check_ = nullptr;
	std::unique_ptr<Sprite> aButton_ = nullptr;

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

	//サウンド
	std::unique_ptr<Sound> checkSound_ = nullptr;
};

