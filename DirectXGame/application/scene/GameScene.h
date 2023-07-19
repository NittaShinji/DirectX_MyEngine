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

	static void StaticInitialize(ImGuiManager* imGuiManager);

	/// <summary>
	/// 初期化
	/// </summary>
	//void Initialize(DirectXBasic* directXBasic,ImGuiManager* imGuiManager);
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

	//ゲーム中のシーン
	enum Game
	{
		TITLE,
		GAME,
		END,
	};

	enum PadNum
	{
		Player1 = 1,
	};

	DirectXBasic* directXBasic_ = nullptr;
	static KeyInput* keys_;
	ImGuiManager* imGuiManager_;

	//デバッグテキスト用の変数
	//ウインドウの表示フラグ
	//bool showEditWindow_ = true;
	
	//ライト
	LightGroup* lightGroup_ = nullptr;

	//カメラ
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Camera> testCamera_ = nullptr;
	std::unique_ptr<GameCamera> testGameCamera_ = nullptr;


	//サウンド
	//std::unique_ptr<Sound> sound_ = nullptr;

	//スプライト
	//std::unique_ptr<Sprite> title_ = nullptr;
	std::unique_ptr<Sprite> test_ = nullptr;
	std::unique_ptr<Sprite> end_ = nullptr;


	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//スプライト共通部分
	//std::unique_ptr<SpriteCommon> spriteCommon_ = nullptr;
	
	//ゲームシーン
	int32_t scene_;

	//レベルデータ
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;
	MODELKEY name_;

	std::map<MODELKEY, Model> models_;

	//当たり判定 球
	Sphere sphereCollision_;
	////当たり判定 平面
	Plane planeCollision_;
	//当たり判定 三角形
	Triangle triangleCollison_;
	//当たり判定 レイ
	Ray rayCollision_;

	//3Dオブジェクト
	//std::unique_ptr<Object3d> sphere_ = nullptr;
	//std::unique_ptr<Object3d> testStage0_ = nullptr;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<Object3d>> objects_;


	//画像なしテストオブジェクト
	std::unique_ptr<Object3d> testObject_ = nullptr;

	//ゲームパッド
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	std::unique_ptr<SphereCollider> sphereCollider_ = nullptr;

	//TouchableObject* objGround_ = nullptr;
	std::unique_ptr<TouchableObject> objGround_ = nullptr;

	//衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;

	
	//待ち時間
	static const int32_t kWaitTime_ = 40;
	//キー入力の時間管理
	int32_t keyTimer_ = 60;

	int32_t hit_;

	XMFLOAT3 move_ = { 0,0,0 };

	const int32_t kActionTime_ = 60;
	int32_t moveTimer_ = kActionTime_;
	bool isUp_ = false;
	bool isDown_ = true;

	XMFLOAT3 trianglePosition2_;
	XMFLOAT3 rayWorldPositon_;

	XMFLOAT3 rotate_ = { 0,0,0 };

	float ambientColor0_[3] = { 1,1,1 };
	//光線方向初期値
	float lightDir0_[3] = { 0,0,1 };
	float lightColor0_[3] = { 1,0,0 };

	float lightDir1_[3] = { 0,1,0 };
	float lightColor1_[3] = { 0,1,0 };

	float lightDir2_[3] = { 1,0,0 };
	float lightColor2_[3] = { 0,0,1 };

	Input* input_ = nullptr;

	XMFLOAT3 move = { 0,0,0 };
	XMFLOAT3 sphereRotation = { 0,0,0 };

	const int actionTime = 720;
	int moveTimer = actionTime;
	bool isUp = false;
	bool isDown = true;

};

