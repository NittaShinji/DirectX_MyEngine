#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "GameScene.h"
#include "PostEffect.h"
#include "xaudio2.h"
#include <fstream>
#include <memory>

class GameManager
{

public: //メンバ関数

	GameManager();

	//初期化
	void Initialize();
	//終了
	void Finalize();
	//毎フレーム更新
	void Update();
	//描画
	void Draw();

	//終了フラグのチェック
	bool IsEndRequst() { return endRequst_; }

private: //メンバ変数

	std::unique_ptr<WindowsAPI> winApi_ = nullptr;

	//DirectX初期化処理
	std::unique_ptr<DirectXBasic> directXBasic_ = nullptr;
	
	//ImGui初期化処理
	std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;
	//入力処理
	std::unique_ptr<Input> input_ = nullptr;
	
	//ゲームシーン
	std::unique_ptr<GameScene> gameScene_ = nullptr;
	
	//スプライト
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

	//ゲーム終了フラグ
	bool endRequst_ = false;
};

