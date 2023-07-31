#include "GameScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "SceneManager.h"
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
	Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("gamescene.wav");
	Sound::GetInstance()->PlaySoundWave("gamescene.wav");

	//ゲームパッド
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//------------コライダー----------
	collisionManager_ = CollisionManager::GetInstance();
	sphereCollider_ = std::make_unique<SphereCollider>();

	//モデル読み込み
	const string sphere = "sphere";
	const string test = "NoImageModel";
	const string testStage0 = "testStage0";
	const string ground = "ground";
	const string skydome = "skydome";

	Model::Load(test);
	Model::Load(sphere);
	Model::Load(testStage0);
	Model::Load(ground);
	Model::Load(skydome);


	//3Dオブジェクトの生成
	XMFLOAT3 spherePosition = { 0,2,20 };
	XMFLOAT3 groundPosition = { 0,-5,0 };

	XMFLOAT3 sphereScale = { 10,10,10 };
	XMFLOAT3 groundScale = { 10,1,10 };
	XMFLOAT3 skydomeScale = { 5,5,10 };


	stage_ = std::make_unique<Stage>();
	stage_->Initialize();

	player_ = Player::Create(sphere);
	player_->SetGamePad(gamePad_.get());

	skydome_ = Object3d::Create(skydome);
	skydome_->SetScale(skydomeScale);

	//------------カメラ----------
	camera_ = std::make_unique<Camera>();
	testCamera_ = std::make_unique<Camera>();
	testGameCamera_ = std::make_unique<GameCamera>();

	//カメラ
	XMFLOAT3 cameraEye = { 30,7.5,-20 };
	XMFLOAT3 testCameraEye = { 0,50,-30 };
	XMFLOAT3 cameraTarget = { 0,5,5 };
	XMFLOAT3 testcameraTarget = { 0,5,5 };

	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize(testCameraEye, testcameraTarget, cameraUp);
	testGameCamera_->Initialize(cameraEye, testcameraTarget, cameraUp);

	//パーティクル
	particleManager_ = ParticleManager::Create();
	playerRunEffect_ = ParticleManager::Create();
}

void GameScene::Update()
{
	if(gamePad_->IsConnected(Player1)) {}

	gamePad_->PushedButtonMoment();
	if(player_->GetIsDead() == false)
	{
		if(KeyInput::HasPushedKey(DIK_SPACE) || gamePad_->GetButtonA())
		{
			player_->SetIsMoving(true);
		}
	}
	else
	{
		player_->Reset();
		testGameCamera_->Reset();
	}

	//光線方向初期値
	static XMVECTOR lightDir = { 1,-1,-10,0 };
	static XMFLOAT3 color = { 1, 1, 1 };

	lightGroup_->SetAmbientColor(color);
	lightGroup_->SetDirLightDir(0, lightDir);
	lightGroup_->SetDirLightColor(0, XMFLOAT3(1, 1, 1));

	{
		//imguiからのライトパラメータを反映
		lightGroup_->SetAmbientColor(XMFLOAT3(ambientColor0_));
		lightGroup_->SetDirLightDir(0, XMVECTOR({ lightDir0_[0], lightDir0_[1], lightDir0_[2], 0 }));
		lightGroup_->SetDirLightColor(0, XMFLOAT3(lightColor0_));
	}

	lightGroup_->Update();

	camera_->Update();
	testCamera_->Update();
	testGameCamera_->Update(player_->GetIsMoving());

	if(player_->GetOnGround() == true)
	{
		for(int i = 0; i < 5; i++)
		{
			//x,y,z全て[-1.0f,+1.0f]でランダムに分布
			const float md_pos = 2.0f;
			XMFLOAT3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 10.0f;
			pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + player_->GetPos().z;
			//x,y,z全て[-0.05f,+0.05f]でランダムに分布
			const float md_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			//重力に見立ててYのみ{-0.001f,0}でランダムに分布
			XMFLOAT3 acc{};
			const float md_acc = 0.001f;
			acc.y = (float)rand() / RAND_MAX * md_acc;
			//色を変化させる
			XMFLOAT4 colorSpeed{};
			colorSpeed.x = 0.0f;
			colorSpeed.y = 0.0f;
			colorSpeed.z = 0.0f;
			colorSpeed.w = 1.0f;

			//追加
			particleManager_->Add(60, pos, vel, acc, colorSpeed, 1.0f, 0.0f);
		}
	}

	for(int i = 0; i < 50; i++)
	{
		//x,y,z全て[-2.0f,+2.0f]でランダムに分布
		const float md_pos = 4.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + stage_->GetGoalPos().z;
		//x,y,z全て[-0.05f,+0.05f]でランダムに分布
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ{-0.001f,0}でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = (float)rand() / RAND_MAX * md_acc;
		//色を変化させる
		XMFLOAT4 colorSpeed{};
		colorSpeed.x = 0.0f;
		colorSpeed.y = 0.0f;
		colorSpeed.z = 0.0f;
		colorSpeed.w = 1.0f;

		//追加
		if(particleManager_->GetIsMaxParticle() == false)
		{
			particleManager_->Add(60, pos, vel, acc, colorSpeed, 1.0f, 0.0f);
		}
	}

	//カメラの切り替え
	if(keys_->HasPushedKey(DIK_0))
	{
		stage_->Update(testCamera_.get());
		player_->Update(testCamera_.get());
		skydome_->Update(testCamera_.get());
		particleManager_->Update(testCamera_.get());
	}
	else
	{
		stage_->Update(testGameCamera_.get());
		player_->Update(testGameCamera_.get());
		skydome_->Update(testGameCamera_.get());
		particleManager_->Update(testGameCamera_.get());
	}

	//スプライトの編集ウインドウの表示
	{
		ImGui::Begin("Light");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(500, 200));

		ImGui::ColorEdit3("ambientColor", ambientColor0_, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("lightDir0", lightDir0_);
		ImGui::ColorEdit3("lightColor0", lightColor0_, ImGuiColorEditFlags_Float);

		ImGui::End();
	}

	if(keyTimer_ < 0)
	{
		if(player_->GetIsDead() == true || player_->GetIsFinish() == true)
		{
			player_->Reset();
			testGameCamera_->Reset();
			keyTimer_ = kWaitTime_;
		}
	}
	else
	{
		keyTimer_--;
	}

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	if(player_->GetIsFinish() == true || keys_->HasPushedKey(DIK_G))
	{
		Sound::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
}

void GameScene::Draw()
{

	//モデル描画
	Object3d::BeforeDraw();
	skydome_->Draw();
	stage_->Draw();
	player_->Draw();

	ParticleManager::PreDraw(directXBasic_->GetCommandList().Get());
	particleManager_->Draw();
}
