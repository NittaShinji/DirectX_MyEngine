#pragma once

#include "DirectXBasic.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"

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
	void Initialize(DirectXBasic* directXBasic);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private:

	DirectXBasic* directXBasic_ = nullptr;
	Input* input_ = nullptr;

	//モデル
	Model testModel_;

	//3Dオブジェクト
	Object3d* object3d_ = nullptr;
	Object3d* nObject3d_ = nullptr;
	Object3d* sObject3d_ = nullptr;


	//スプライト
	Sprite* title_ = nullptr;

	//スプライト共通部分
	SpriteCommon* spriteCommon_ = nullptr;



};

