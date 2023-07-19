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

void GameScene::StaticInitialize(ImGuiManager* imGuiManager)
{
	//imGuiManager_ = imGuiManager;
}

//void GameScene::Initialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
void GameScene::Initialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;
	scene_ = GAME;

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
	Sound::GetInstance()->LoadSoundWave("Alarm01.wav");
	Sound::GetInstance()->PlaySoundWave("Alarm01.wav");

	//ゲームパッド
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//------------コライダー----------

	collisionManager_ = CollisionManager::GetInstance();
	sphereCollider_ = std::make_unique<SphereCollider>();


	//-----------読み込み---------------

	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile("Stage0.json");

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成

		if(objectData.fileName == "sphere" || objectData.fileName == "testStage0")
		{
			//モデルをロード
			Model::Load(objectData.fileName);

			//3Dオブジェクトの生成
			std::unique_ptr<TouchableObject> newObject = nullptr;
			newObject = TouchableObject::Create(objectData.fileName);

			DirectX::XMFLOAT3 pos;
			pos = objectData.translation;
			newObject->SetTransform(pos);

			//回転角
			DirectX::XMFLOAT3 rot;
			rot = objectData.rotation;
			newObject->SetRotation(rot);

			//大きさ
			DirectX::XMFLOAT3 scale;
			scale = objectData.scaling;
			newObject->SetScale(scale);

			//配列に登録
			objects_.push_back(std::move(newObject));
		}
	}

	//------------画像読み込み----------
	//title_ = std::make_unique<Sprite>();
	test_ = std::make_unique<Sprite>();
	end_ = std::make_unique<Sprite>();
	//spriteCommon_ = std::make_unique<SpriteCommon>();
	//spriteCommon_ = SpriteCommon::GetInstance();
	//SpriteCommon::GetInstance()->Initialize(directXBasic_);

	//スプライト関係初期化
	//spriteCommon_->Initialize(directXBasic_);
	//画像読み込み
	//spriteCommon_->LoadTexture("title.png");
	SpriteCommon::GetInstance()->LoadTexture("tomas.png");
	//SpriteCommon::GetInstance()->LoadTexture("title.png");
	SpriteCommon::GetInstance()->LoadTexture("end.png");


	//SpriteCommon::LoadTexture("tomas.png");
	//SpriteCommon::LoadTexture("title.png");
	//SpriteCommon::LoadTexture("end.png");


	//Sprite::StaticInitialize();

	//個々の画像を初期化(指定した番号の画像を使用する)
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	XMFLOAT2 testPosition = { 100,100 };
	XMFLOAT2 testSize = { 266,369 };
	XMFLOAT2 endPosition = { 0,0 };
	XMFLOAT2 endSize = { 1280,720 };

	//title_->Initialize(titlePosition, titleSize);
	test_->Initialize(testPosition, testSize);
	end_->Initialize(endPosition, endSize);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	//spriteCommon_->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//スプライト用のパイプラインステート(透過可能)を用意
	//spriteCommon_->SemiTransparent();

	//------------モデル読み込み----------

	//モデル読み込み
	const string sphere = "sphere";
	const string test = "NoImageModel";
	const string spear = "spear";
	const string testStage0 = "testStage0";
	const string ground = "ground";

	Model::Load(test);
	Model::Load(sphere);
	Model::Load(spear);
	Model::Load(testStage0);
	Model::Load(ground);

	//3Dオブジェクトの生成
	XMFLOAT3 spherePosition = { 0,2,20 };
	XMFLOAT3 groundPosition = { 0,-5,0 };

	XMFLOAT3 sphereScale = { 10,10,10 };
	XMFLOAT3 groundScale = { 10,1,10 };

	player_ = Player::Create(sphere);
	player_->SetGamePad(gamePad_.get());
	
	/*sphere_ = Object3d::Create(sphere);
	sphere_->Initialize();
	sphere_->SetTransform(spherePosition);
	sphere_->SetCollider(sphereCollider_.get());*/

	objGround_ = TouchableObject::Create(ground);
	objGround_->SetTransform(groundPosition);
	objGround_->SetScale(groundScale);

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
	static XMVECTOR lightDir = { 0,1,5,0 };
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

	lightGroup_->SetDirLightDir(1, lightDir);
	lightGroup_->SetDirLightColor(1, XMFLOAT3(1, 1, 1));

	lightGroup_->SetDirLightDir(2, lightDir);
	lightGroup_->SetDirLightColor(2, XMFLOAT3(1, 1, 1));

	{
		//imguiからのライトパラメータを反映
		lightGroup_->SetAmbientColor(XMFLOAT3(ambientColor0_));
		lightGroup_->SetDirLightDir(0, XMVECTOR({ lightDir0_[0], lightDir0_[1], lightDir0_[2], 0 }));
		lightGroup_->SetDirLightColor(0, XMFLOAT3(lightColor0_));
		lightGroup_->SetDirLightDir(1, XMVECTOR({ lightDir1_[0], lightDir1_[1], lightDir1_[2], 0 }));
		lightGroup_->SetDirLightColor(1, XMFLOAT3(lightColor1_));
		lightGroup_->SetDirLightDir(2, XMVECTOR({ lightDir2_[0], lightDir2_[1], lightDir2_[2], 0 }));
		lightGroup_->SetDirLightColor(2, XMFLOAT3(lightColor2_));
	}

	lightGroup_->Update();

	switch(scene_)
	{

	case TITLE:

		//アンカーポイントの設定
		XMFLOAT2 anchorPoint = { 0.0f,0.0f };
		//title_->SetAnchorPoint(anchorPoint);
		//title_->matUpdate();
		test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();

		if(keyTimer_ < 0)
		{
			if(KeyInput::HasPushedKey(DIK_SPACE))
			{
				scene_ = GAME;
				keyTimer_ = kWaitTime_;
				//player_->SetIsMoving(true);
			}
		}
		else
		{
			keyTimer_--;
		}

		break;

	case GAME:

		camera_->Update();
		testCamera_->Update();
		testGameCamera_->Update(player_->GetIsMoving());

		//カメラの切り替え
		if(keys_->HasPushedKey(DIK_0))
		{
			for(auto& object : objects_)
			{
				object->Update(testCamera_.get());
			}
			//objGround_->Update(testCamera_.get());
			player_->Update(testCamera_.get());
			//sphere_->Update(testCamera_.get());
		}
		else
		{
			for(auto& object : objects_)
			{
				object->Update(testGameCamera_.get());
			}

			//sphere_->Update(testGameCamera_.get());
			objGround_->Update(testGameCamera_.get());
			player_->Update(testGameCamera_.get());
		}

		//画像の更新処理

		//スプライトの編集ウインドウの表示
		{
			ImGui::Begin("Light");
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::SetWindowSize(ImVec2(500, 200));

			ImGui::ColorEdit3("ambientColor", ambientColor0_, ImGuiColorEditFlags_Float);
			ImGui::InputFloat3("lightDir0", lightDir0_);
			ImGui::ColorEdit3("lightColor0", lightColor0_, ImGuiColorEditFlags_Float);
			ImGui::InputFloat3("lightDir1", lightDir1_);
			ImGui::ColorEdit3("lightColor1", lightColor1_, ImGuiColorEditFlags_Float);
			ImGui::InputFloat3("lightDir2", lightDir2_);
			ImGui::ColorEdit3("lightColor2", lightColor2_, ImGuiColorEditFlags_Float);

			ImGui::End();
		}

		if(keyTimer_ < 0)
		{
			if(player_->GetIsFinish() == true)
			{
				/*player_->Reset();
				testGameCamera_->Reset();
				scene_ = TITLE;
				keyTimer_ = kWaitTime_;*/
			}
		}
		else
		{
			keyTimer_--;
		}

		//全ての衝突をチェック
		collisionManager_->CheckAllCollisions();

		break;

	case END:

		end_->SetAnchorPoint(anchorPoint);
		end_->matUpdate();

		if(keyTimer_ < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer_ = kWaitTime_;
			}
		}
		else
		{
			keyTimer_--;
		}

		break;

	default:
		break;
	}
}

void GameScene::Draw()
{
	switch(scene_)
	{

	case TITLE:

		//画像描画
		SpriteCommon::GetInstance()->BeforeDraw();
		SpriteCommon::GetInstance()->Update();

		//spriteCommon_->BeforeDraw();
		//spriteCommon_->Update();
		//title_->Draw("title.png");


		break;

	case GAME:

		//モデル描画
		Object3d::BeforeDraw();

		for(auto& object : objects_)
		{
			object->Draw();
		}

		player_->Draw();
		//sphere_->Draw();
		//objGround_->Draw();
		

		break;
	case END:

		//画像描画
		SpriteCommon::GetInstance()->BeforeDraw();
		SpriteCommon::GetInstance()->Update();

		//spriteCommon_->BeforeDraw();
		//spriteCommon_->Update();
		end_->Draw("end.png");

		break;

	default:
		break;
	}
}
