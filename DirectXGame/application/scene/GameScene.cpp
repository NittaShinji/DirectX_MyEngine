#include "GameScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::Initialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);
	Mesh::StaticInitialize(directXBasic_);

	//------------ライト------------
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());

	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//------------サウンド----------
	//sound_ = std::make_unique<Sound>();
	//sound_->Initialize();
	//Sound::GetInstance()->LoadSoundWave("Alarm01.wav");
	//Sound::GetInstance()->PlaySoundWave("Alarm01.wav");

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

	//skydome_ = std::make_unique<TouchableObject>();
	//skydome_ = TouchableObject::Create(skydome);
	//skydome_->SetScale(skydomeScale);

	//------------カメラ----------
	Camera::StaticInitialize(directXBasic_);
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
}

void GameScene::Update()
{
	if(gamePad_->IsConnected(Player1)){}

	if(player_->GetIsDead() == false)
	{
		if(KeyInput::HasPushedKey(DIK_SPACE))
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

	if(keys_->HasPushedKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
	else if(keys_->HasPushedKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; }
	if(keys_->HasPushedKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
	else if(keys_->HasPushedKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; }

	if(keys_->HasPushedKey(DIK_2)) { color.x -= 0.01f; }
	else if(keys_->HasPushedKey(DIK_3)) { color.x += 0.01f; }
	else if(keys_->HasPushedKey(DIK_4)) { color.y += 0.01f; }
	else if(keys_->HasPushedKey(DIK_5)) { color.y -= 0.01f; }

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
	
	//カメラの切り替え
	if(keys_->HasPushedKey(DIK_0))
	{
		stage_->Update(testCamera_.get());
		player_->Update(testCamera_.get());
		skydome_->Update(testCamera_.get());
	}
	else
	{
		stage_->Update(testGameCamera_.get());
		player_->Update(testGameCamera_.get());
		skydome_->Update(testGameCamera_.get());
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
}

void GameScene::Draw()
{
	//モデル描画
	Object3d::BeforeDraw();
	skydome_->Draw();
	stage_->Draw();
	player_->Draw();

}
