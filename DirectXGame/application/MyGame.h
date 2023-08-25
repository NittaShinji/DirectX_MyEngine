#pragma once
#include "ImGuiManager.h"
#include "GameScene.h"
#include "PostEffect.h"
#include "MyFramework.h"
#include "TitleScene.h"
#include <fstream>
#include <memory>

class MyGame : public MyFramework
{

public: //メンバ関数

	MyGame();

	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム更新
	void Update() override;
	//描画
	void Draw() override;

private: //メンバ変数

	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

	//ライト
	LightGroup* lightGroup_ = nullptr;

	//背景
	std::unique_ptr<BackGround> backGround_ = nullptr;
	//std::unique_ptr<Object3d> titleSphere_ = nullptr;

	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;

	std::unique_ptr<Camera> testCamera_ = nullptr;

};

