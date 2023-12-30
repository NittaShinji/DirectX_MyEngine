#pragma once
#include "GameScene.h"
#include "PostEffect.h"
#include "MyFramework.h"
#include "TitleScene.h"
#include <fstream>
#include <memory>
#include "ImGuiManager.h"

/// <summary>
/// MyGameクラス
/// このゲームの大本となるクラス
/// </summary>
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
};

