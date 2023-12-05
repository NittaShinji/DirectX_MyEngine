#include "TutorialScene.h"
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

DirectXBasic* TutorialScene::directXBasic_ = nullptr;
ImGuiManager* TutorialScene::imGuiManager_ = nullptr;

TutorialScene::TutorialScene() {}
TutorialScene::~TutorialScene() {}

void TutorialScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	//ParticleManager::StaticInitialize(directXBasic_->GetDevice().Get());
}

void TutorialScene::Initialize()
{
	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//------------サウンド----------
	/*Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("gamescene.wav");
	Sound::GetInstance()->PlaySoundWave("gamescene.wav",true);*/

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

	//const Vector2 aButtonSize = { 64.0f,64.0f };
	const Vector2 aButtonSize = { TextureManager::GetInstance()->GetTexSize("A.png")};

	Vector2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_)-(aButtonSize.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y * 4) + (aButtonSize.y / 2);

	const Vector2 bButtonSize = { TextureManager::GetInstance()->GetTexSize("B.png") };
	Vector2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_)-(bButtonSize.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (bButtonSize.y * 4) + (bButtonSize.y / 2);

	aButtonSprite_->Initialize("A.png",aButtonPosition);
	bButtonSprite_->Initialize("B.png",bButtonPosition);

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

	//------------カメラ----------
	gameCamera_ = std::make_unique<GameCamera>();

	//カメラ
	gameCamera_->Initialize();

	//パーティクル
	/*particleManager_ = ParticleManager::Create();
	particleManager_->SetGenerationNum(50);
	playerRunEffect_ = ParticleManager::Create();
	playerRunEffect_->SetGenerationNum(50);*/

	isShowingButtonA = false;
}

void TutorialScene::Update()
{
	Tutorial1();

	//スプライト更新処理
	aButtonSprite_->matUpdate();
	bButtonSprite_->matUpdate();


	playerPosX = player_->GetTransform().x;
	playerPosY = player_->GetTransform().y;
	playerPosZ = player_->GetTransform().z;

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
		player_->Reset(gameCamera_.get());
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
	//gameCamera_->Update(player_->GetIsMoving(), player_->GetIsDead(), player_->GetTotalAxcell(),player_->GetPos());

	if(player_->GetOnGround() == true)
	{
		for(int i = 0; i < 5; i++)
		{
			//x,y,z全て[-1.0f,+1.0f]でランダムに分布
			const float md_pos = 2.0f;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 10.0f + player_->GetTransform().y - 1.0f;
			pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + player_->GetTransform().z;
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
			if(player_->GetAttributeColor() == Attribute::pink)
			{
				Vector4 colorSpeed{ 1.0f,1.0f,1.0f,1.0f };
			}
			else if(player_->GetAttributeColor() == Attribute::yellow)
			{
				Vector4 colorSpeed{ 1.0f,1.0f,1.0f,1.0f };
			}

			//色を変化させる
			Vector4 colorSpeed{ 1.0f,-1.0f,-1.0f,1.0f };

			//追加
			/*if(particleManager_->GetIsMaxParticle() == false)
			{
				particleManager_->Add(60, pos, vel, acc, colorSpeed, 2.0f, 1.0f);
			}*/
		}
	}

	for(int i = 0; i < 10; i++)
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
		/*if(particleManager_->GetIsMaxParticle() == false)
		{
			particleManager_->Add(60, pos, vel, acc, colorSpeed, 1.0f, 0.0f);
		}*/
	}

	//カメラの切り替え
	stage_->Update(gameCamera_.get(),player_->GetRightAxcell());
	player_->Update(gameCamera_.get());
	skydome_->Update(gameCamera_.get());
	backGround_->Update(gameCamera_.get());

	//particleManager_->Update(gameCamera_.get(), player_->GetAttributeColor());

#ifdef _DEBUG

	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();

#endif

	if(keyTimer_ < 0)
	{
		if(player_->GetIsDead() == true || player_->GetIsFinish() == true)
		{
			player_->Reset(gameCamera_.get());
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

	if(player_->GetIsFinish() == true || keys_->PushedKeyMoment(DIK_G))
	{
		//Sound::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
}

void TutorialScene::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();
	//背景オブジェクトの描画
	Object3d::BeforeDraw();
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

	//ParticleManager::PreDraw(directXBasic_->GetCommandList().Get());
	//particleManager_->Draw();

	SpriteCommon::GetInstance()->BeforeDraw();

	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");

	//デバッグテキストの描画
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

void TutorialScene::Tutorial1()
{
	Vector3 cameraEye = gameCamera_->GetEye();
	if(cameraEye.z > 50 && cameraEye.z < 90)
	{
		isShowingButtonA = true;
	}
}
