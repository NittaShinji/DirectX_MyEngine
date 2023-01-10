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

class GameScene
{
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMMATRIX = DirectX::XMMATRIX;

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

	//当たり判定のチェック
	void CheckAllCollision();


private:

	enum Game
	{
		TITLE,
		GAME,
		END,
	};

	DirectXBasic* directXBasic_ = nullptr;
	KeyInput* keys_;

	//カメラ
	Camera* camera_ = nullptr;
	Camera* testCamera_ = nullptr;

	//サウンド
	Sound* sound = nullptr;

	//スプライト
	Sprite* title_ = nullptr;

	//スプライト共通部分
	SpriteCommon* spriteCommon_ = nullptr;

	//モデル
	Model testModel_;

	//3Dオブジェクト
	Object3d* object3d_ = nullptr;
	Object3d* nObject3d_ = nullptr;
	Object3d* sObject3d_ = nullptr;

	//ゲームシーン
	int scene_;

	//AABBの衝突判定フラグ
	bool AABBisHit_;

};

