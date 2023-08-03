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
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "LevelManager.h"
#include "GameCamera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "BaseScene.h"
#include "Stage.h"
#include "ParticleManager.h"

class CollisionManager;
class TouchableObject;
class Player;

class GameScene : public BaseScene
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

	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;
	
	//ライト
	LightGroup* lightGroup_ = nullptr;

	//カメラ
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Camera> testCamera_ = nullptr;
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//3Dオブジェクト
	std::unique_ptr<Stage> stage_ = nullptr;

	//背景
	std::unique_ptr<Object3d> skydome_ = nullptr;

	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//コライダー
	std::unique_ptr<SphereCollider> sphereCollider_ = nullptr;
	std::unique_ptr<TouchableObject> objGround_ = nullptr;

	//衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;

	//待ち時間
	static const int32_t kWaitTime_ = 40;
	//キー入力の時間管理
	int32_t keyTimer_ = 60;

	//float ambientColor0_[3] = { 1,1,1 };
	Vector3 ambientColor0_ = { 1,1,1 };
	//光線方向初期値
	//float lightDir0_[3] = { 1,-5,-5 };
	Vector3 lightDir0_ = { 1,-5,-5 };

	//float lightColor0_[3] = { 1,1,1 };
	Vector3 lightColor0_ = { 1,1,1 };

	//float whiteColor_[3] = { 1,1,1 };
	Vector3 whiteColor_ = { 1,1,1 };

	//パーティクル
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;
	std::unique_ptr<ParticleManager> playerRunEffect_ = nullptr;
};

