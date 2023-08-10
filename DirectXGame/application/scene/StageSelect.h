#include "BaseScene.h"
#include "Sprite.h"
#include "GamePad.h"
#pragma once

class StageSelectScene : public BaseScene
{

public:

	//静的初期化
	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	std::unique_ptr<Sprite> selectSprite_ = nullptr;
	std::unique_ptr<Sprite> BackGroundLeft = nullptr;


	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//シーンを変えるかどうか
	bool isChangeScene_;
	
	const float kChangeSceneTime_ = 10.0f;
	float changeSceneTimer_ = kChangeSceneTime_;
};
