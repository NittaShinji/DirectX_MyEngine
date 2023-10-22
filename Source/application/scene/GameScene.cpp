#include "GameScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "SceneManager.h"
#include "ObjParticleManager.h"
#include "Vector4.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

DirectXBasic* GameScene::directXBasic_ = nullptr;
ImGuiManager* GameScene::imGuiManager_ = nullptr;

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	ParticleManager::GetInstance()->StaticInitialize(directXBasic_->GetDevice().Get(), directXBasic_->GetCommandList().Get());
	ObjParticleManager::GetInstance()->StaticInitialize(directXBasic_->GetDevice().Get(), directXBasic_->GetCommandList().Get());
}

void GameScene::Initialize()
{
	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	//------------サウンド----------
	//Sound::GetInstance()->Initialize();
	//Sound::GetInstance()->LoadSoundWave("gamescene.wav");
	//Sound::GetInstance()->PlaySoundWave("gamescene.wav",true);

	//サウンド
	SoundManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->LoadSoundWave("gamescene.wav");

	gameSound_ = std::make_unique<Sound>();
	gameSound_->Initialize("gamescene.wav");
	gameSound_->PlaySoundWave(true);

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
	backGroundSprite_ = std::make_unique<Sprite>();

	TextureManager::GetInstance()->LoadTexture("jump.png");
	TextureManager::GetInstance()->LoadTexture("arrow.png");

	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;
	const Vector2 backGroundSize = { backGroundWidth,backGroundHeight };

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
	backGroundSprite_->Initialize(Vector2(0.0f, 0.0f), backGroundSize);

	//モデル読み込み
	const string sphere = "sphere";
	const string test = "NoImageModel";
	const string testStage0 = "testStage0";
	const string ground = "ground";
	const string skydome = "skydome";
	const string cube = "Cube";
	const string plane = "Plane";
	const string wall = "wall";

	Model::Load(testStage0);
	Model::Load(cube);
	Model::Load(wall);

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
	gameCamera_ = std::make_unique<GameCamera>();

	//カメラ
	Vector3 cameraEye = { 30,7.5,-20 };

	Vector3 testCameraEye = { 0,50,-30 };
	Vector3 cameraTarget = { 0,5,5 };

	Vector3 testcameraTarget = { 0,5,5 };

	Vector3 cameraUp = { 0,1,0 };

	gameCamera_->Initialize(cameraEye, testcameraTarget, cameraUp);

	//パーティクル
	/*groundParticle_ = GroundParticle::Create();
	ParticleManager::GetInstance()->AddEmitter(groundParticle_.get());
	ParticleManager::GetInstance()->Initialize();*/

	blockParticle_ = BlockParticle::Create();
	blockParticle_->Preparation(sphere);
	ObjParticleManager::GetInstance()->AddEmitter(blockParticle_.get());
	ObjParticleManager::GetInstance()->Initialize();

	isReset_ = false;
}

void GameScene::Update()
{
	if(player_->GetIsDead() == true)
	{
		if(isReset_ == false)
		{
			//blockParticle_->Preparation(player_->GetPos(), "sphere");
			isReset_ = true;
		}

		if(blockParticle_->GetParticleNum() == 0)
		{
			ParticleManager::GetInstance()->ParticleRemove();
			ObjParticleManager::GetInstance()->ParticleRemove();
			stage_->Reset("Stage0.json");
			gameCamera_->Reset();
			player_->Reset(gameCamera_.get());
			isReset_ = false;
		}
	}

	//スプライト
  	aButtonSprite_->matUpdate();
	bButtonSprite_->matUpdate();
	jumpSprite_->matUpdate();
	arrowSprite_->matUpdate();
	backGroundSprite_->matUpdate();

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

	gameCamera_->Update(player_->GetIsMoving(),player_->GetIsDead(), player_->GetTotalAxcell());

	if(player_->GetOnGround() == true)
	{
		//groundParticle_->Preparation(player_->GetPos(), player_->GetAttributeColor());
		blockParticle_->PopUpdate(gameCamera_.get(), player_->GetPos());
	}
	else
	{
		
	}

	//カメラの切り替え
	player_->Update(gameCamera_.get());
	stage_->Update(gameCamera_.get(), player_->GetRightAxcell());
	skydome_->Update(gameCamera_.get());
	plane_->Update(gameCamera_.get());
	backGround_->Update(gameCamera_.get());

	//ParticleManager::GetInstance()->Update(gameCamera_.get(), player_->GetAttributeColor());
	ObjParticleManager::GetInstance()->Update(gameCamera_.get());

#ifdef _DEBUG

	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();

#endif

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	if(player_->GetIsFinish() == true)
	{
		ParticleManager::GetInstance()->ParticleRemove();
		//ObjParticleManager::GetInstance()->ParticleRemove();
		stage_->Reset("Stage0.json");
		player_->Reset(gameCamera_.get());
		gameCamera_->Reset();
	
		SoundManager::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

#ifdef _DEBUG
	if(keys_->PushedKeyMoment(DIK_G))
	{
		SoundManager::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
#endif
}

void GameScene::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();

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

	//ParticleManager::GetInstance()->Draw();
	ObjParticleManager::GetInstance()->Draw();

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
