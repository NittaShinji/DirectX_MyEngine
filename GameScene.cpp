#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace DirectX;

KeyInput* GameScene::keys_ = nullptr;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//サウンド
	sound->Finalize();
	delete sound;
	//画像
	delete spriteCommon_;
	delete title_;
	delete test_;
	//モデル
	delete object3d_;
	delete nObject3d_;
	delete sObject3d_;
	delete camera_;
	delete testCamera_;
	//delete directXBasic_;

	sound = nullptr;
	title_ = nullptr;
	test_ = nullptr;
	//モデル
	object3d_ = nullptr;
	nObject3d_ = nullptr;
	sObject3d_ = nullptr;
	camera_ = nullptr;
	testCamera_ = nullptr;
}

void GameScene::Initialize(DirectXBasic* directXBasic,ImGuiManager* imGuiManager)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
	imGuiManager_ = imGuiManager;
	scene_ = GAME;

	//------------サウンド----------

	//Sound::StaticInitialize();
	sound = new Sound;
	sound->Initialize();
	sound->LoadSoundWave("Alarm01.wav");
	//sound->PlaySoundWave("Alarm01.wav");

	//------------画像読み込み----------
	title_ = new Sprite;
	test_ = new Sprite;

	spriteCommon_ = new SpriteCommon;
	//スプライト関係初期化
	spriteCommon_->Initialize(directXBasic_);
	Sprite::StaticInitialize(spriteCommon_);

	//画像読み込み
	Sprite::LoadTexture("tomas.png");
	Sprite::LoadTexture("black.png");

	//個々の画像を初期化(指定した番号の画像を使用する)
	XMFLOAT2 titlePosition = { 400,400 };
	XMFLOAT2 titleSize = { 400,400 };
	title_->Initialize(1,titlePosition, titleSize);

	XMFLOAT2 testPosition = { 400,400 };
	XMFLOAT2 testSize = { 100,100 };
	test_->Initialize(2, testPosition, testSize);

	//シェーダー読み込み
	spriteCommon_->ShaderLoad();
	//スプライト用のパイプラインステート(透過可能)を用意
	spriteCommon_->SemiTransparent();

	//------------モデル読み込み----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//モデル読み込み
	const string testModelName = "triangle_tex";
	const string testModelName2 = "block2";
	const string testModelName3 = "pillar";

	Model::Load(testModelName);
	Model::Load(testModelName2);
	Model::Load(testModelName3);

	//3Dオブジェクトの生成
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 position2 = { 30,0,0 };
	XMFLOAT3 position3 = { -30,0,0 };

	object3d_ = new Object3d(testModelName, position);
	nObject3d_ = new Object3d(testModelName2, position2);
	sObject3d_ = new Object3d(testModelName3, position3);

	//------------カメラ----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = new Camera;
	testCamera_ = new Camera;
	XMFLOAT3 cameraEye = { 0,0,-100 };
	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize({30,0,-50}, cameraTarget, cameraUp);

	//--球の初期値を設定
	//中心座標
	sphere_.center = XMVectorSet(0, 2, 0, 1);
	sphere_.radius = 1.0f;

	//平面の初期値を設定
	//法線ベクトル
	plane_.normal = XMVectorSet(0, 1, 0, 0);
	//原点(0,0,0)からの距離
	plane_.distance = 0.0f;
}

void GameScene::Update()
{
	switch (scene_)
	{

	case TITLE:

		break;

	case GAME:
		camera_->Updata();
		testCamera_->Updata();

		
		//カメラの切り替え
		if (keys_->HasPushedKey(DIK_0))
		{
			object3d_->Update(testCamera_);
			nObject3d_->Update(testCamera_);
			sObject3d_->Update(testCamera_);
		}
		else
		{
			//モデルの更新処理
			object3d_->Update(camera_);
			nObject3d_->Update(camera_);
			sObject3d_->Update(camera_);
		}
		
		//画像の更新処理
		//アンカーポイントの設定
		XMFLOAT2 anchorPoint = { 0.0f,0.0f };
		title_->SetAnchorPoint(anchorPoint);
		title_->matUpdate();

		/*test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();*/
		
		//スプライトの編集ウインドウの表示
		{
			static float pos[2] = { title_->GetPosition().x ,title_->GetPosition().x };
			
			ImGui::Begin("Edit");
			ImGui::SetWindowPos("Edit", ImVec2(100, 100));
			ImGui::SetWindowSize("Edit", ImVec2(500, 100));
			ImGui::SliderFloat2("position", pos, 0.0f, 1000.0f);
			title_->SetPosition(XMFLOAT2(pos));

			if(ImGui::Button("Close Me"))
				showEditWindow = false;

			ImGui::End();
		}

		break;

	case END:

		break;

	default:
		break;
	}
}

void GameScene::Draw()
{
	switch (scene_)
	{

	case TITLE:



		break;

	case GAME:

		//モデル描画
		object3d_->BeforeDraw();
		//object3d_->Draw();
		nObject3d_->Draw();
		sObject3d_->Draw();

		//画像描画
		spriteCommon_->BeforeDraw();
		spriteCommon_->Update();
		title_->Draw();
		test_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
