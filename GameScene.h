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
	
	//ライト
	LightGroup* lightGroup_ = nullptr;

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
	//Object3d* object3d_ = nullptr;
	//std::vector<Object3d*> objects;
	std::vector<std::unique_ptr<Object3d>> objects;

	
	//ゲームシーン
	int scene_;

	//レベルデータ
	//LevelManager* levelManager_ = nullptr;
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;

	MODELKEY name_;

	std::map<MODELKEY, Model> models_;

	//当たり判定 球
	Sphere sphereCollision;
	////当たり判定 平面
	Plane planeCollision;
	//当たり判定 三角形
	Triangle triangleCollison;
	//当たり判定 レイ
	Ray rayCollision;

	Object3d* sphere_;
	Object3d* ground_;
	Object3d* triangle_;
	Object3d* ray_;

	Object3d* blender_;

	//待ち時間
	static const int waitTime = 40;
	//キー入力の時間管理
	int keyTimer = 60;

	int hit;

	XMFLOAT3 move = { 0,0,0 };

	const int actionTime = 60;
	int moveTimer = actionTime;
	bool isUp = false;
	bool isDown = true;

	XMFLOAT3 trianglePosition2;
	XMFLOAT3 rayWorldPositon;

	XMFLOAT3 rotate = { 0,0,0 };

	float ambientColor0[3] = { 1,1,1 };
	//光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

};

