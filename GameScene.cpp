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
	//モデル
	delete camera_;
	delete testCamera_;
	delete sphere_;
	delete ground_;
	delete triangle_;

	//delete directXBasic_;

	sound = nullptr;
	title_ = nullptr;
	test_ = nullptr;
	//モデル
	object3d_ = nullptr;
	camera_ = nullptr;
	testCamera_ = nullptr;
	sphere_ = nullptr;
	ground_ = nullptr;
	triangle_ = nullptr;

}

void GameScene::Initialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
	imGuiManager_ = imGuiManager;
	scene_ = TITLE;

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
	Sprite::LoadTexture("title.png");
	
	//個々の画像を初期化(指定した番号の画像を使用する)
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	title_->Initialize(1, titlePosition, titleSize);

	//シェーダー読み込み
	spriteCommon_->ShaderLoad();
	//スプライト用のパイプラインステート(透過可能)を用意
	spriteCommon_->SemiTransparent();

	//------------モデル読み込み----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//モデル読み込み
	
	const string sphere = "sphere";
	const string ground = "ground";
	const string testTriangle = "triangle_tex";

	Model::Load(sphere);
	Model::Load(ground);
	Model::Load(testTriangle);

	//3Dオブジェクトの生成
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 spherePosition = { 0,0,0 };
	XMFLOAT3 groundPosition = { 0,-5,0 };
	XMFLOAT3 trianglePosition = { 0,0,0 };


	XMFLOAT3 sphereScale = { 10,10,10 };
	XMFLOAT3 groundScale = { 20,5,20 };
	XMFLOAT3 triangleScale = { 20,20,1 };


	sphere_ = new Object3d(sphere, spherePosition, sphereScale);
	ground_ = new Object3d(ground, groundPosition, groundScale);
	triangle_ = new Object3d(testTriangle, trianglePosition, triangleScale);

	//------------カメラ----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = new Camera;
	testCamera_ = new Camera;
	XMFLOAT3 cameraEye = { 0,0,-100 };
	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize({ 50,0,10 }, cameraTarget, cameraUp);

	//--球の初期値を設定
	//中心座標
	sphereCollision.pos = sphere_->GetWorldPos();
	sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };
	sphereCollision.radius = 5.0f;
	sphere_->SetTransform(sphereCollision.pos);

	//平面の初期値を設定
	//法線ベクトル
	planeCollision.normal = XMVectorSet(0, 1, 0, 0);
	//原点(0,0,0)からの距離
	planeCollision.distance = -5.0f;

	//三角形の初期値を設定
	//ラジアン90度　1.5708f　
	//XMFLOAT3 triangleRotation = { 1.5708f,0.0f,0.0f };
	//triangle_->SetRotation(triangleRotation);
	XMFLOAT3 trianglePosition2 = triangle_->GetWorldPos();

	triangleCollison.p0 = XMVectorSet(trianglePosition2.x - triangleScale.x - 5.0f, trianglePosition2.y -triangleScale.y - 5.0f, trianglePosition2.z, 1); //左手前
	triangleCollison.p1 = XMVectorSet(trianglePosition2.x , trianglePosition2.y + triangleScale.y - 5.0f, trianglePosition2.z, 1); //奥
	triangleCollison.p2 = XMVectorSet(trianglePosition2.x +  triangleScale.x + 5.0f, trianglePosition2.y - triangleScale.y - 5.0f, trianglePosition2.z, 1); //右手前
	triangleCollison.normal = XMVectorSet(0.0f, 0.0f, 1.0f, 0); //上向き

}

void GameScene::Update()
{
	switch(scene_)
	{

	case TITLE:

		//アンカーポイントの設定
		XMFLOAT2 anchorPoint = { 0.0f,0.0f };
		title_->SetAnchorPoint(anchorPoint);
		title_->matUpdate();

		if(keyTimer < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = GAME;
				keyTimer = waitTime;
			}
		}
		else
		{
			keyTimer--;
		}

		break;

	case GAME:
		camera_->Updata();
		testCamera_->Updata();

		//カメラの切り替え
		if(keys_->HasPushedKey(DIK_0))
		{
			sphere_->Update(testCamera_);
			ground_->Update(testCamera_);
			triangle_->Update(testCamera_);
		}
		else
		{
			//モデルの更新処理
			sphere_->Update(camera_);
			ground_->Update(camera_);
			triangle_->Update(camera_);
		}

		//画像の更新処理

		/*test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();*/

		/*if(moveTimer >= 0)
		{
			moveTimer--;
		}
		else if(moveTimer < 0)
		{
			moveTimer = actionTime;
		}

		if(moveTimer >= 30)
		{
			isDown = false;
			isUp = true;
			move.y = 0;
		}
		else if(moveTimer <= 29)
		{
			isUp = false;
			isDown = true;
			move.y = 0;
		}

		if(isDown == true && isUp == false)
		{
			move.y -= 0.3f;
		}
		else if(isUp == true && isDown == false)
		{
			move.y += 0.3f;
		}*/

		if(keys_->HasPushedKey(DIK_W))
		{
			move.z -= 0.1f;
		}
		else if(keys_->HasReleasedKey(DIK_W) != keys_->HasPushedKey(DIK_S))
		{
			move.z = 0;
		}
		else if(keys_->HasPushedKey(DIK_S))
		{
			move.z += 0.1f;
		}
		else if(keys_->HasReleasedKey(DIK_S)&& keys_->HasPushedKey(DIK_W))
		{
			move.z = 0;
		}

		if(keys_->HasPushedKey(DIK_A))
		{
			move.y -= 0.1f;
		}
		else if(keys_->HasReleasedKey(DIK_A) != keys_->HasPushedKey(DIK_D))
		{
			move.y = 0;
		}
		else if(keys_->HasPushedKey(DIK_D))
		{
			move.y += 0.1f;
		}
		else if(keys_->HasReleasedKey(DIK_D)&& keys_->HasPushedKey(DIK_A))
		{
			move.y = 0;
		}


		sphereCollision.pos.y += move.y;
		sphereCollision.pos.z += move.z;

		sphere_->SetTransform(sphereCollision.pos);
		sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };

		/*hit = Collision::CheckSphere2Plane(sphereCollision, planeCollision);
		sphere_->SetColorFlag(hit);*/

		//球と三角形の当たり判定
		XMVECTOR inter;
		hit = Collision::CheckSphere2Triangle(sphereCollision, triangleCollison, &inter);


		//スプライトの編集ウインドウの表示
		{

			ImGui::Begin("Collision");
			ImGui::SetWindowSize("Collision", ImVec2(500, 100));
			ImGui::InputInt("hit", &hit, 0.0f, 1000.0f);
			ImGui::End();
		}


		if(keyTimer < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer = waitTime;
			}
		}
		else
		{
			keyTimer--;
		}

		break;

	case END:

		if(keyTimer < 0)
		{
			if(keys_->HasPushedKey(DIK_SPACE))
			{

				scene_ = TITLE;
				keyTimer = waitTime;
			}
		}
		else
		{
			keyTimer--;
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
		title_->Draw();

		break;

	case GAME:

		//モデル描画
		sphere_->BeforeDraw();
		//ground_->BeforeDraw();

		sphere_->Draw();
		//ground_->Draw();
		triangle_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
