#pragma once

#include "Sprite.h"
#include "Object3d.h"
#include "Sound.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
#include "LevelManager.h"
#include "GameCamera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "BaseScene.h"
#include "Stage.h"
#include "ParticleManager.h"
#include "BackGround.h"
#include "PostEffect.h"
#include "SoundManager.h"

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

	static void StaticInitialize();

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

	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

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
	std::unique_ptr<Object3d> plane_ = nullptr;
	std::unique_ptr<BackGround> backGround_ = nullptr;

	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//コライダー
	std::unique_ptr<SphereCollider> sphereCollider_ = nullptr;
	std::unique_ptr<TouchableObject> objGround_ = nullptr;

	//衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;

	//待ち時間
	static const int32_t kWaitTime_ = 40;

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

	//UIスプライト
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> jumpSprite_ = nullptr;

	std::unique_ptr<Sprite> bButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> arrowSprite_ = nullptr;

	std::unique_ptr<Sprite> backGroundSprite_ = nullptr;

	float startScale = 1.0f;
	float endScale = 2.0f;

	float imGuiPos[3]{ 0.0f,0.0f,0.0f };
	float imGuiVel[3]{ 0.0f,0.0f,0.0f };
	float imGuiAcc[3]{ 0.0f,0.0f,0.0f };

	//サウンド
	std::unique_ptr<Sound> gameSound_ = nullptr;
	
};

