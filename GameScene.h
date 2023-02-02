#pragma once

#include "DirectXBasic.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "Sound.h"
#include "CollisionPrimitive.h"
#include "ImGuiManager.h"

class GameScene
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXBasic* directXBasic,ImGuiManager* imGuiManager);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private:

	//ゲーム中のシーン
	enum Game
	{
		TITLE,
		GAME,
		END,
	};

	DirectXBasic* directXBasic_ = nullptr;
	static KeyInput* keys_;
	ImGuiManager* imGuiManager_ = nullptr;

	//デバッグテキスト用の変数
	//ウインドウの表示フラグ
	bool showEditWindow = true;
	
	//カメラ
	Camera* camera_ = nullptr;
	Camera* testCamera_ = nullptr;

	//サウンド
	Sound* sound = nullptr;

	//スプライト
	Sprite* title_ = nullptr;
	Sprite* test_ = nullptr;


	//スプライト共通部分
	SpriteCommon* spriteCommon_ = nullptr;

	//モデル
	Model testModel_;

	//3Dオブジェクト
	Object3d* object3d_ = nullptr;
	
	//ゲームシーン
	int scene_;

	//当たり判定 球
	//Sphere sphere_;
	////当たり判定 平面
	//Plane plane_;

	Object3d* sphere_;
	Object3d* ground_;

	//待ち時間
	static const int waitTime = 60;
	//キー入力の時間管理
	int keyTimer = 60;


};

