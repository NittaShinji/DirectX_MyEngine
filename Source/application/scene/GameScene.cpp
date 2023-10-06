#include "GameScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "SceneManager.h"
#include "Vector4.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

DirectXBasic* GameScene::directXBasic_ = nullptr;
ImGuiManager* GameScene::imGuiManager_ = nullptr;

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	ParticleManager::StaticInitialize(directXBasic_->GetDevice().Get());
}

void GameScene::Initialize()
{

	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//------------サウンド----------
	/*Sound::GetInstance()->Initialize();*/
	//Sound::GetInstance()->LoadSoundWave("gamescene.wav");
	//Sound::GetInstance()->PlaySoundWave("gamescene.wav",true);

	//ゲームパッド
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//------------コライダー----------
	collisionManager_ = CollisionManager::GetInstance();
	sphereCollider_ = std::make_unique<SphereCollider>();

	//ポストエフェクト初期化処理
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(directXBasic_);

	//スプライト
	aButtonSprite_ = std::make_unique<Sprite>();
	bButtonSprite_ = std::make_unique<Sprite>();
	jumpSprite_ = std::make_unique<Sprite>();
	arrowSprite_ = std::make_unique<Sprite>();

	TextureManager::GetInstance()->LoadTexture("jump.png");
	TextureManager::GetInstance()->LoadTexture("arrow.png");

	const Vector2 aButtonSize = { 64.0f,64.0f };
	Vector2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_)-(aButtonSize.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y * 4) + (aButtonSize.y / 2);

	const Vector2 bButtonSize = { 64.0f,64.0f };
	Vector2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_)-(bButtonSize.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (bButtonSize.y * 4) + (bButtonSize.y / 2);

	const Vector2 jumpSpriteSize = { 64.0f,64.0f };
	Vector2 jumpSpritePosition;
	jumpSpritePosition.x = (WindowsAPI::kWindow_width_)-(jumpSpriteSize.x * 2);
	jumpSpritePosition.y = (WindowsAPI::kWindow_height_ / 2) + (jumpSpriteSize.y * 3) + (jumpSpriteSize.y / 2);

	const Vector2 arrowSize = { 64.0f,64.0f };
	Vector2 arrowPosition;
	arrowPosition.x = (WindowsAPI::kWindow_width_)-(arrowSize.x);
	arrowPosition.y = (WindowsAPI::kWindow_height_ / 2) + (arrowSize.y * 3) + (arrowSize.y / 2);

	aButtonSprite_->Initialize(aButtonPosition, aButtonSize);
	bButtonSprite_->Initialize(bButtonPosition, bButtonSize);
	jumpSprite_->Initialize(jumpSpritePosition, jumpSpriteSize);
	arrowSprite_->Initialize(arrowPosition, arrowSize);

	//モデル読み込み
	const string sphere = "sphere";
	const string test = "NoImageModel";
	const string testStage0 = "testStage0";
	const string ground = "ground";
	const string skydome = "skydome";
	const string cube = "Cube";
	const string plane = "Plane";

	Model::Load(sphere);
	Model::Load(testStage0);
	Model::Load(skydome);
	Model::Load(cube);
	Model::Load(plane);

	//3Dオブジェクトの生成

	stage_ = std::make_unique<Stage>();
	stage_->Initialize("Stage0.json");

	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	player_ = Player::Create(sphere);
	player_->SetGamePad(gamePad_.get());

	skydome_ = Object3d::Create(skydome);
	Vector3 skydomeScale = { 5,5,10 };
	skydome_->SetScale(skydomeScale);

	plane_ = Object3d::Create(plane);
	Vector3 planeScale = { 500,1000,1000 };
	plane_->SetScale(planeScale);


	//------------カメラ----------
	camera_ = std::make_unique<Camera>();
	testCamera_ = std::make_unique<Camera>();
	gameCamera_ = std::make_unique<GameCamera>();

	//カメラ
	Vector3 cameraEye = { 30,7.5,-20 };

	Vector3 testCameraEye = { 0,50,-30 };
	Vector3 cameraTarget = { 0,5,5 };

	Vector3 testcameraTarget = { 0,5,5 };

	Vector3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize(testCameraEye, testcameraTarget, cameraUp);
	gameCamera_->Initialize(cameraEye, testcameraTarget, cameraUp);

	//パーティクル
	particleManager_ = ParticleManager::Create();
	particleManager_->SetGenerationNum(50);
	playerRunEffect_ = ParticleManager::Create();
	playerRunEffect_->SetGenerationNum(50);
}

void GameScene::Update()
{
	//スプライト
	aButtonSprite_->matUpdate();
	bButtonSprite_->matUpdate();
	jumpSprite_->matUpdate();
	arrowSprite_->matUpdate();

	if(gamePad_->IsConnected(Player1)) {}

	gamePad_->PushedButtonMoment();
	if(player_->GetIsDead() == false)
	{
		if(gamePad_->GetButtonA())
		{
			player_->SetIsMoving(true);
		}

		if(keys_->PushedKeyMoment(DIK_SPACE))
		{
			player_->SetIsMoving(true);
		}
	}
	else
	{
		player_->Reset();
		gameCamera_->Reset();
	}

	//光線方向初期値
	static Vector3 lightDir = { 1,-1,-10 };
	float lightDirUp = 0.0f;

	static Vector3 color = { 1, 1, 1 };

	lightGroup_->SetAmbientColor(color);
	lightGroup_->SetDirLightDir(0, lightDir, lightDirUp);
	lightGroup_->SetDirLightColor(0, Vector3(1, 1, 1));

	{
		//imguiからのライトパラメータを反映
		lightGroup_->SetAmbientColor(Vector3(ambientColor0_));
		lightGroup_->SetDirLightDir(0, Vector3({ lightDir0_.x, lightDir0_.y, lightDir0_.z }), 0.0f);

		lightGroup_->SetDirLightColor(0, Vector3(lightColor0_));
	}

	lightGroup_->Update();

	camera_->Update();
	testCamera_->Update();
	gameCamera_->Update(player_->GetIsMoving(), player_->GetTotalAxcell(), player_->GetPlayerInitPos());

	if(player_->GetOnGround() == true)
	{
		for(int i = 0; i < 5; i++)
		{
			//x,y,z全て[-1.0f,+1.0f]でランダムに分布
			const float md_pos = 2.0f;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 10.0f + player_->GetPos().y - 1.0f;
			pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + player_->GetPos().z;
			//x,y,z全て[-0.05f,+0.05f]でランダムに分布
			const float md_vel = 0.1f;
			Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			//重力に見立ててYのみ{-0.001f,0}でランダムに分布
			Vector3 acc{};
			const float md_acc = 0.001f;
			acc.y = (float)rand() / RAND_MAX * md_acc;

			//色を変化させる
			if(player_->GetAttribute() == Attribute::pink)
			{
				Vector4 colorSpeed{ 1.0f,1.0f,1.0f,1.0f };
			}
			else if(player_->GetAttribute() == Attribute::yellow)
			{
				Vector4 colorSpeed{ 1.0f,1.0f,1.0f,1.0f };
			}

			//色を変化させる
			Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

			//追加
			if(particleManager_->GetIsMaxParticle() == false)
			{
				particleManager_->Add(60, pos, vel, acc, colorSpeed, 2.0f, 1.0f);
			}
		}
	}

	for(int i = 0; i < 5; i++)
	{
		//x,y,z全て[-2.0f,+2.0f]でランダムに分布
		const float md_pos = 4.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + stage_->GetGoalPos().z;
		//x,y,z全て[-0.05f,+0.05f]でランダムに分布
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ{-0.001f,0}でランダムに分布
		Vector3 acc{};
		const float md_acc = 0.01f;
		acc.y = (float)rand() / RAND_MAX * md_acc;
		//色を変化させる
		Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

		//追加
		if(particleManager_->GetIsMaxParticle() == false)
		{
			particleManager_->Add(60, pos, vel, acc, colorSpeed, 1.0f, 0.0f);
		}
	}

	//カメラの切り替え
	stage_->Update(gameCamera_.get());
	player_->Update(gameCamera_.get());
	skydome_->Update(gameCamera_.get());
	plane_->Update(gameCamera_.get());
	backGround_->Update(gameCamera_.get());

	particleManager_->Update(gameCamera_.get(), player_->GetAttribute());

#ifdef _DEBUG

	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();

#endif

	if(keyTimer_ < 0)
	{
		if(player_->GetIsDead() == true || player_->GetIsFinish() == true)
		{
			player_->Reset();
			gameCamera_->Reset();
			keyTimer_ = kWaitTime_;
		}
	}
	else
	{
		keyTimer_--;
	}

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	if(player_->GetIsFinish() == true)
	{
		//Sound::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

#ifdef _DEBUG
	if(keys_->PushedKeyMoment(DIK_G))
	{
		//Sound::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
#endif
}

void GameScene::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();
	//背景オブジェクトの描画
	Object3d::BeforeDraw();
	plane_->Draw();
	backGround_->Draw();
	postEffect_->PostDrawScene();

	//描画開始
	directXBasic_->BeforeDraw();

	//ポストエフェクトの描画
	postEffect_->Draw();

	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	//モデル描画
	Object3d::BeforeDraw();
	stage_->Draw();
	player_->Draw();

	ParticleManager::PreDraw(directXBasic_->GetCommandList().Get());
	particleManager_->Draw();

	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	aButtonSprite_->Update();
	bButtonSprite_->Update();
	jumpSprite_->Update();
	arrowSprite_->Update();

	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");
	jumpSprite_->Draw("jump.png");
	arrowSprite_->Draw("arrow.png");

	//デバッグテキストの描画
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}
