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

class CollisionManager;
class TouchableObject;
class Player;

/// <summary>
/// チュートリアルシーン
/// </summary>
class TutorialScene : public BaseScene
{
public:

	//コンストラクタ
	TutorialScene();
	//デストラクタ
	~TutorialScene();

	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

	//チュートリアル1
	void Tutorial1();

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

	//ライト
	LightGroup* lightGroup_ = nullptr;

	//カメラ
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//3Dオブジェクト
	std::unique_ptr<Stage> stage_ = nullptr;

	//背景
	std::unique_ptr<Object3d> skydome_ = nullptr;
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
	//キー入力の時間管理
	int32_t keyTimer_ = 60;

	Vector3 ambientColor0_ = { 1,1,1 };
	//光線方向初期値
	Vector3 lightDir0_ = { 1,-5,-5 };
	Vector3 lightColor0_ = { 1,1,1 };
	Vector3 whiteColor_ = { 1,1,1 };

	float playerPosX;
	float playerPosY;
	float playerPosZ;

	//UIスプライト
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> bButtonSprite_ = nullptr;

	//画像ボタンの表示
	bool isShowingButtonA;
	bool isShowingButtonB;
	//スロー用のフラグ
	bool isSlowing;
};

