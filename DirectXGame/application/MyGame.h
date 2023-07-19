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

	//ImGui初期化処理
	//std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;
	////基盤シーン
	//std::unique_ptr<BaseScene> scene_ = nullptr;
	////タイトルシーン
	//std::unique_ptr<TitleScene> titleScene_ = nullptr;
	////ゲームシーン
	//std::unique_ptr<GameScene> gameScene_ = nullptr;
	//
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = nullptr;
};

