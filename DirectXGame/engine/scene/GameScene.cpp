#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = directXBasic;
	imGuiManager_ = imGuiManager;
	scene_ = TITLE;

	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//------------ライト------------
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());

	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//------------サウンド----------
	sound_ = std::make_unique<Sound>();
	sound_->Initialize();
	//sound_->LoadSoundWave("Alarm01.wav");
	//sound_->PlaySoundWave("Alarm01.wav");

	
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
			std::unique_ptr<Object3d> newObject = std::make_unique<Object3d>();

			/*XMMATRIX matTrans = XMMatrixIdentity();
			XMMATRIX matRot = XMMatrixIdentity();
			XMMATRIX matScale = XMMatrixIdentity();
			XMMATRIX matWolrd = XMMatrixIdentity();
			*/
			//座標
			//matTrans = objectData.translation;
			//newObject->SetMatTrans(matTrans);

			DirectX::XMFLOAT3 pos;
			pos = objectData.translation;
			newObject->SetTransform(pos);

			//回転角
			DirectX::XMFLOAT3 rot;
			rot = objectData.rotation;
			newObject->SetRotation(rot);

			//matRot = objectData.rotation;
			//newObject->SetMatRot(matRot);
			
			//大きさ
			
			//matScale = objectData.scaling;
			//newObject->SetMatScale(matScale);

			DirectX::XMFLOAT3 scale;
			scale = objectData.scaling;
			newObject->SetScale(scale);

			newObject->Initialize(objectData.fileName,pos,rot,scale);

			//配列に登録
			objects_.push_back(std::move(newObject));
		}
	}

	

	//------------画像読み込み----------
	title_ = std::make_unique<Sprite>();
	test_ = std::make_unique<Sprite>();
	//spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_ = SpriteCommon::GetInstance();

	//スプライト関係初期化
	spriteCommon_->Initialize(directXBasic_);
	//画像読み込み
	//spriteCommon_->LoadTexture("title.png");
	SpriteCommon::LoadTexture("tomas.png");
	SpriteCommon::LoadTexture("title.png");

	Sprite::StaticInitialize();

	//個々の画像を初期化(指定した番号の画像を使用する)
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	XMFLOAT2 testPosition = { 100,100 };
	XMFLOAT2 testSize = { 266,369 };

	title_->Initialize(titlePosition, titleSize);
	test_->Initialize(testPosition, testSize);

	//シェーダー読み込み
	spriteCommon_->ShaderLoad();
	//スプライト用のパイプラインステート(透過可能)を用意
	spriteCommon_->SemiTransparent();

	//------------モデル読み込み----------

	//モデル読み込み
	const string sphere = "sphere";
	const string test = "NoImageModel";
	const string spear = "spear";
	const string testStage0 = "testStage0";


	Model::Load(test);
	Model::Load(sphere);
	Model::Load(spear);
	Model::Load(testStage0);

	
	//3Dオブジェクトの生成
	XMFLOAT3 sphereScale = { 10,10,10 };
	
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 spherePosition = { 0,0,0 };
	
	
	sphere_ = std::make_unique<Object3d>();
	sphere_->Initialize(sphere, XMFLOAT3(10, 0, 0), XMFLOAT3(0, 0, 0), sphereScale);
	testStage0_ = std::make_unique<Object3d>();
	testStage0_->Initialize(testStage0, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(5, 5, 5));

	/*sphere_->Initialize(sphere);
	sphere_->SetTransform(XMFLOAT3(40, 0, 0));
	sphere_->SetRotation(XMFLOAT3(0, 0, 0));
	sphere_->SetScale(sphereScale);*/

	testObject_ = std::make_unique<Object3d>();
	testObject_->Initialize(test, XMFLOAT3(-10, 0, 0), XMFLOAT3(0, 0, 0), sphereScale);

	/*testObject_->Initialize(sphere);
	testObject_->SetTransform(XMFLOAT3(-40, 0, 0));
	testObject_->SetRotation(XMFLOAT3(0, 0, 0));
	testObject_->SetScale(sphereScale);*/

	//------------カメラ----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = std::make_unique<Camera>();
	//camera_ = new Camera;
	testCamera_ = std::make_unique<Camera>();


	//カメラ
	XMFLOAT3 cameraEye = { 0,20,-60 };
	XMFLOAT3 testCameraEye = { 0,50,-30 };
	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };


	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize(testCameraEye, cameraTarget, cameraUp);


}

void GameScene::Update()
{
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
		title_->SetAnchorPoint(anchorPoint);
		title_->matUpdate();
		test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();

		if(keyTimer_ < 0)
		{
			if(KeyInput::HasPushedKey(DIK_SPACE))
			{

				scene_ = GAME;
				keyTimer_ = kWaitTime_;
			}
		}
		else
		{
			keyTimer_--;
		}

		break;

	case GAME:

		camera_->Updata();
		testCamera_->Updata();

		//カメラの切り替え
		if(keys_->HasPushedKey(DIK_0))
		{
			for(auto& object : objects_)
			{
				object->Update(testCamera_.get());
			}

			testObject_->Update(testCamera_.get());
			testStage0_->Update(testCamera_.get());
			sphere_->Update(testCamera_.get());
		}
		else
		{
			//アンカーポイントの設定
			for(auto& object : objects_)
			{
				object->Update(camera_.get());
			}

			testObject_->Update(camera_.get());
			testStage0_->Update(camera_.get());

			//モデルの更新処理
			sphere_->Update(camera_.get());
		}

		if(moveTimer >= 0)
		{
			moveTimer--;
		}
		else if(moveTimer < 0)
		{
			moveTimer = actionTime;
		}

		if(moveTimer >= 360)
		{
			isDown = false;
			isUp = true;
			move.y = 0;
		}
		else if(moveTimer <= 359)
		{
			isUp = false;
			isDown = true;
			move.y = 0;
		}

		if(isDown == true && isUp == false)
		{
			move.y -= 0.01f;
		}
		else if(isUp == true && isDown == false)
		{
			move.y += 0.01f;
		}

		sphereRotation.y += move.y;
		sphere_->SetRotation(sphereRotation);
		testObject_->SetRotation(sphereRotation);

		
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

	case END:

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
		spriteCommon_->BeforeDraw();
		spriteCommon_->Update();
		title_->Draw("title.png");

		break;

	case GAME:

		//モデル描画
		Object3d::BeforeDraw();

		//sphere_->Draw();
		//testObject_->Draw();
		//testStage0_->Draw();

		for(auto& object : objects_)
		{
			object->Draw();
		}

		break;
	case END:

		break;

	default:
		break;
	}
}
