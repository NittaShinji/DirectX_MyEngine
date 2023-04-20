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
	////サウンド
	sound->Finalize();
	delete sound;
	//画像
	delete spriteCommon_;
	delete title_;
	//delete test_;
	//モデル
	delete camera_;
	delete testCamera_;
	delete sphere_;
	delete ground_;
	delete triangle_;

	//delete directXBasic_;

	//sound = nullptr;
	spriteCommon_ = nullptr;
	title_ = nullptr;
	//test_ = nullptr;
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
	//imGuiManager_ = imGuiManager;
	scene_ = TITLE;

	//------------サウンド----------

	//Sound::StaticInitialize();
	sound = new Sound;
	sound->Initialize();
	sound->LoadSoundWave("Alarm01.wav");
	//sound->PlaySoundWave("Alarm01.wav");

	//------------画像読み込み----------
	title_ = new Sprite;
	spriteCommon_ = new SpriteCommon;

	//スプライト関係初期化
	spriteCommon_->Initialize(directXBasic_);
	Sprite::StaticInitialize(spriteCommon_);
	spriteCommon_->LoadTexture(1, "title.png");

	//画像読み込み
	//Sprite::LoadTexture("title.png");
	
	//個々の画像を初期化(指定した番号の画像を使用する)
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	title_->Initialize(1, titlePosition, titleSize, spriteCommon_);

	//シェーダー読み込み
	spriteCommon_->ShaderLoad();
	//スプライト用のパイプラインステート(透過可能)を用意
	spriteCommon_->SemiTransparent();

	//------------モデル読み込み----------
	//Model::StaticInitialize(directXBasic_);
	//Object3d::StaticInitialize(directXBasic_);

	////モデル読み込み
	//
	//const string sphere = "sphere";
	//const string ground = "ground";
	//const string testTriangle = "triangle_tex";
	//const string ray = "blackCube";

	//Model::Load(sphere);
	//Model::Load(ground);
	//Model::Load(testTriangle);
	//Model::Load(ray);

	////3Dオブジェクトの生成
	//XMFLOAT3 sphereScale = { 10,10,10 };
	//XMFLOAT3 groundScale = { 20,3,20 };
	//XMFLOAT3 triangleScale = { 20,1,20 };
	//XMFLOAT3 rayScale = { 1,1,1 };

	//XMFLOAT3 position = { 0,0,0 };
	//XMFLOAT3 spherePosition = { 0,0,0 };
	//XMFLOAT3 groundPosition = { 0,0,0 };
	//XMFLOAT3 trianglePosition = { 0,0,0 };
	////XMFLOAT3 raySetPosition = { 0,rayScale.y * 2,rayScale.z * 2 };
	//XMFLOAT3 raySetPosition = { 0,rayScale.y,0 };

	//sphere_ = new Object3d(sphere, spherePosition, sphereScale);
	//ground_ = new Object3d(ground, groundPosition, groundScale);
	//triangle_ = new Object3d(testTriangle, trianglePosition, triangleScale);
	//ray_ = new Object3d(ray, raySetPosition, rayScale);

	//------------カメラ----------
	Camera::StaticInitialize(directXBasic_);
	camera_ = new Camera;
	testCamera_ = new Camera;
	XMFLOAT3 cameraEye = { 0,60,-30 };
	XMFLOAT3 cameraTarget = { 0,0,0 };
	XMFLOAT3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);
	testCamera_->Initialize({ 50,60,-30 }, cameraTarget, cameraUp);

	//------------当たり判定----------
	//--球の初期値を設定
	//中心座標
	//sphereCollision.pos = sphere_->GetWorldPos();
	//sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };
	//sphereCollision.radius = 10.0f;
	//sphere_->SetTransform(sphereCollision.pos);

	////平面の初期値を設定
	////法線ベクトル
	//planeCollision.normal = XMVectorSet(0, 1, 0, 0);
	////原点(0,0,0)からの距離
	//planeCollision.distance = 0.0f;

	////三角形の初期値を設定
	////ラジアン90度　1.5708f　
	////XMFLOAT3 triangleRotation = { 1.5708f,0.0f,0.0f };
	////triangle_->SetRotation(triangleRotation);
	//trianglePosition2 = triangle_->GetWorldPos();

	//triangleCollison.p0 = XMVectorSet(trianglePosition2.x - triangleScale.x, trianglePosition2.y, trianglePosition2.z - triangleScale.z, 1); //左手前
	//triangleCollison.p1 = XMVectorSet(trianglePosition2.x - triangleScale.x, trianglePosition2.y, trianglePosition2.z + triangleScale.z, 1); //奥
	//triangleCollison.p2 = XMVectorSet(trianglePosition2.x + triangleScale.x, trianglePosition2.y, trianglePosition2.z - triangleScale.z, 1); //右手前

	//triangleCollison.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0); //上向き

	////レイ
	////3Dオブジェクトの座標を取得
	//rayWorldPositon = ray_->GetWorldPos();
	////レイの初期値を設定
	//rayCollision.start = XMVectorSet(rayWorldPositon.x, rayWorldPositon.y, rayWorldPositon.z, 1);//原点やや上
	//rayCollision.dir = XMVectorSet(0, -1, 0, 0);//下向き
	////斜め向きにする場合は正規化する。

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

		////カメラの切り替え
		//if(keys_->HasPushedKey(DIK_0))
		//{
		//	sphere_->Update(testCamera_);
		//	ground_->Update(testCamera_);
		//	triangle_->Update(testCamera_);
		//	ray_->Update(testCamera_);
		//}
		//else
		//{
		//	//モデルの更新処理
		//	sphere_->Update(camera_);
		//	ground_->Update(camera_);
		//	triangle_->Update(camera_);
		//	ray_->Update(camera_);
		//}

		//画像の更新処理

		/*test_->SetAnchorPoint(anchorPoint);
		test_->matUpdate();*/

		///*if(moveTimer >= 0)
		//{
		//	moveTimer--;
		//}
		//else if(moveTimer < 0)
		//{
		//	moveTimer = actionTime;
		//}

		//if(moveTimer >= 30)
		//{
		//	isDown = false;
		//	isUp = true;
		//	move.y = 0;
		//}
		//else if(moveTimer <= 29)
		//{
		//	isUp = false;
		//	isDown = true;
		//	move.y = 0;
		//}

		//if(isDown == true && isUp == false)
		//{
		//	move.y -= 0.3f;
		//}
		//else if(isUp == true && isDown == false)
		//{
		//	move.y += 0.3f;
		//}*/

		///*if(keys_->HasPushedKey(DIK_W))
		//{
		//	move.z -= 0.1f;
		//}
		//else if(keys_->HasReleasedKey(DIK_W) != keys_->HasPushedKey(DIK_S))
		//{
		//	move.z = 0;
		//}
		//else if(keys_->HasPushedKey(DIK_S))
		//{
		//	move.z += 0.1f;
		//}
		//else if(keys_->HasReleasedKey(DIK_S)&& keys_->HasPushedKey(DIK_W))
		//{
		//	move.z = 0;
		//}

		//if(keys_->HasPushedKey(DIK_A))
		//{
		//	move.x -= 0.1f;
		//}
		//else if(keys_->HasReleasedKey(DIK_A) != keys_->HasPushedKey(DIK_D))
		//{
		//	move.x = 0;
		//}
		//else if(keys_->HasPushedKey(DIK_D))
		//{
		//	move.x += 0.1f;
		//}
		//else if(keys_->HasReleasedKey(DIK_D)&& keys_->HasPushedKey(DIK_A))
		//{
		//	move.x = 0;
		//}

		//if(keys_->HasPushedKey(DIK_Q))
		//{
		//	move.y -= 0.1f;
		//}
		//else if(keys_->HasReleasedKey(DIK_Q) != keys_->HasPushedKey(DIK_E))
		//{
		//	move.y = 0;
		//}
		//else if(keys_->HasPushedKey(DIK_E))
		//{
		//	move.y += 0.1f;
		//}
		//else if(keys_->HasReleasedKey(DIK_E)&& keys_->HasPushedKey(DIK_Q))
		//{
		//	move.y = 0;
		//}*/


		////レイ操作

		//rayWorldPositon = ray_->GetWorldPos();

		//{
		//	float moveZ = 0.1f;
		//	if(keys_->HasPushedKey(DIK_W)) { rayWorldPositon.z += moveZ; }
		//	else if(keys_->HasPushedKey(DIK_S)) { rayWorldPositon.z -= moveZ; }

		//	float moveY = 0.1f;
		//	if(keys_->HasPushedKey(DIK_Q)) { rayWorldPositon.y += moveY; }
		//	else if(keys_->HasPushedKey(DIK_E)) { rayWorldPositon.y -= moveY; }

		//	float moveX = 0.1f;
		//	if(keys_->HasPushedKey(DIK_A)) { rayWorldPositon.x -= moveX; }
		//	else if(keys_->HasPushedKey(DIK_D)) { rayWorldPositon.x += moveX; }


		//}

		////レイの当たり判定を更新
		////rayCollision.dir = XMVectorSet(0, -1, 0, 0);//下向き
		//ray_->SetTransform(rayWorldPositon);

		//rayCollision.start = XMVectorSet(rayWorldPositon.x, rayWorldPositon.y, rayWorldPositon.z, 1);//原点やや上
		//rayCollision.dir = XMVectorSet(0, -1, 0, 0);//下向き
		///*sphereCollision.pos.x += move.x;
		//sphereCollision.pos.y += move.y;
		//sphereCollision.pos.z += move.z;*/

		///*sphere_->SetTransform(sphereCollision.pos);
		//sphereCollision.center = { sphere_->GetWorldPos().x,sphere_->GetWorldPos().y,sphere_->GetWorldPos().z,1 };*/

		//trianglePosition2 = triangle_->GetWorldPos();

		//

		////球と三角形の当たり判定
		///*XMVECTOR inter;
		//hit = Collision::CheckSphere2Triangle(sphereCollision, triangleCollison, &inter);*/
		////レイと平面の当たり判定
		//XMVECTOR inter;
		//float distance;
		////hit = Collision::CheckRay2Plane(rayCollision, planeCollision, &distance, &inter);
		//hit = Collision::CheckRay2Triangle(rayCollision, triangleCollison, &distance, &inter);

		////スプライトの編集ウインドウの表示
		//{

		//	ImGui::Begin("Collision");
		//	ImGui::SetWindowSize("Collision", ImVec2(500, 100));
		//	ImGui::InputInt("hit", &hit, 0.0f, 1000.0f);

		//	ImGui::End();
		//}


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
		//sphere_->BeforeDraw();
		//ground_->BeforeDraw();

		//sphere_->Draw();
		//ground_->Draw();
		//triangle_->Draw();
		//ray_->Draw();

		break;
	case END:

		break;

	default:
		break;
	}
}
