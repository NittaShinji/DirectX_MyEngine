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
#include "CollisionAttribute.h"
#include "Vector4.h"
#include "Easing.h"
#include <sstream>
#include <iomanip>
#include <string>
#include "GameSpeed.h"
#include "GameTimer.h"

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
	sceneTransitionUp_ = std::make_unique<Sprite>();
	sceneTransitionDown_ = std::make_unique<Sprite>();
	//testParticleSprite_ = std::make_unique<Sprite>();

	//黒色のテクスチャ―
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_ / 2 , Vector4(0.0f, 0.0f, 0.0f, 1.0f), "BlackBackGroundHalfTex");
	TextureManager::GetInstance()->LoadTexture("jump.png");
	TextureManager::GetInstance()->LoadTexture("arrow.png");
	TextureManager::GetInstance()->LoadTexture("effect1.png");
	TextureManager::GetInstance()->LoadTexture("effect2.png");
	TextureManager::GetInstance()->LoadTexture("jumpEffect6.png");
	TextureManager::GetInstance()->LoadTexture("backGround.png");


	Vector2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_)-(kUiSize_.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (kUiSize_.y * 4) + (kUiSize_.y / 2);

	Vector2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_)-(kUiSize_.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (kUiSize_.y * 4) + (kUiSize_.y / 2);

	Vector2 jumpSpritePosition;
	jumpSpritePosition.x = (WindowsAPI::kWindow_width_)-(kUiSize_.x * 2);
	jumpSpritePosition.y = (WindowsAPI::kWindow_height_ / 2) + (kUiSize_.y * 3) + (kUiSize_.y / 2);

	Vector2 arrowPosition;
	arrowPosition.x = (WindowsAPI::kWindow_width_)-(kUiSize_.x);
	arrowPosition.y = (WindowsAPI::kWindow_height_ / 2) + (kUiSize_.y * 3) + (kUiSize_.y / 2);

	Vector2 transitionSize;
	transitionSize.x = (WindowsAPI::kWindow_width_);
	transitionSize.y = (WindowsAPI::kWindow_height_ / 2);

	aButtonSprite_->Initialize("A.png",aButtonPosition);
	aButtonSprite_->SetSize(Vector2(kUiSize_));
	bButtonSprite_->Initialize("B.png",bButtonPosition);
	bButtonSprite_->SetSize(Vector2(kUiSize_));
	jumpSprite_->Initialize("jump.png",jumpSpritePosition);
	jumpSprite_->SetSize(Vector2(kUiSize_));
	arrowSprite_->Initialize("arrow.png",arrowPosition);
	arrowSprite_->SetSize(Vector2(kUiSize_));
	backGroundSprite_->Initialize("backGround.png",Vector2(0.0f, 0.0f));
	sceneTransitionUp_->Initialize("BlackBackGroundHalfTex",Vector2(0.0f, 0.0f));
	sceneTransitionDown_->Initialize("BlackBackGroundHalfTex",Vector2(0.0f, WindowsAPI::kWindow_height_ / 2));
	//testParticleSprite_->Initialize(Vector2(kHalfWindowWidth, kHalfWindowHeight));

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

	plane_ = TouchableObject::Create(plane, COLLISION_ATTR_BLACK);
	Vector3 planeScale = { 500,1000,1000 };
	Vector3 planeTransform = { 0.0f,-3.0f,0.0f };
	plane_->SetAttributeColor(black);
	plane_->SetTransform(planeTransform);
	plane_->SetScale(planeScale);

	//------------カメラ----------
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();

	//2Dパーティクル
	groundParticle_ = GroundParticle::Create("effect1.png");
	secondJumpParticle_ = SecondJump2DParticle::Create("jumpEffect6.png");
	ParticleManager::GetInstance()->AddEmitter(groundParticle_.get());
	ParticleManager::GetInstance()->AddEmitter(secondJumpParticle_.get());
	ParticleManager::GetInstance()->Initialize();
	//3Dパーティクル
	landParticle_ = LandParticle::Create(cube);
	deadParticle_ = DeadParticle::Create(cube);
	breakParticle_ = BreakParticle::Create(cube);
	ObjParticleManager::GetInstance()->AddEmitter(landParticle_.get());
	ObjParticleManager::GetInstance()->AddEmitter(deadParticle_.get());
	ObjParticleManager::GetInstance()->AddEmitter(breakParticle_.get());

	gameSpeed_ = std::make_unique<GameSpeed>();
	gameSpeed_->Initialize();
	player_->SetGameSpeed(gameSpeed_.get());
	gameCamera_->SetGameSpeed(gameSpeed_.get());
	
	secondJumpParticle_->SetGameSpeed(gameSpeed_.get());
	groundParticle_->SetGameSpeed(gameSpeed_.get());
	landParticle_->SetGameSpeed(gameSpeed_.get());
	deadParticle_->SetGameSpeed(gameSpeed_.get());
	breakParticle_->SetGameSpeed(gameSpeed_.get());

	GameTimer::GetInstance()->InGameInitialize();
	
	isReset_ = false;
}

void GameScene::Update()
{
	SceneAnimation();

#ifdef _DEBUG
	if(keys_->PushedKeyMoment(DIK_N))
	{
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
	}
	else if(keys_->PushedKeyMoment(DIK_H))
	{
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::HARD);
	}
	else if(keys_->PushedKeyMoment(DIK_S))
	{
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::SLOW);
	}
	else if(keys_->PushedKeyMoment(DIK_ESCAPE))
	{
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::STOP);
	}
#endif

	if(player_->GetIsDead() == true)
	{
		if(isReset_ == false)
		{
			isReset_ = true;
		}

		ParticleManager::GetInstance()->ParticleRemove();

		if(deadParticle_->GetCanReset() == true)
		{
			ObjParticleManager::GetInstance()->ParticleReset(gameCamera_.get());
			stage_->Reset("Stage0.json");
			gameCamera_->Initialize();
			player_->Reset(gameCamera_.get());
			isReset_ = false;
			deadParticle_->SetCanReset(false);
			deadParticle_->Reset();
			GameTimer::GetInstance()->Reset();
			ResetSceneAnimation();
		}
	}

	//スプライト
	aButtonSprite_->matUpdate();
	bButtonSprite_->matUpdate();
	jumpSprite_->matUpdate();
	arrowSprite_->matUpdate();
	backGroundSprite_->matUpdate();
	sceneTransitionUp_->matUpdate();
	sceneTransitionDown_->matUpdate();
	//testParticleSprite_->matUpdate();

	

	if(gamePad_->IsConnected(Player1)) {}

	gamePad_->PushedButtonMoment();
	if(player_->GetIsDead() == false)
	{
		if(gamePad_->GetButtonA())
		{
			player_->SetIsMoving(true);
		}

		if(keys_->PushedKeyMoment(DIK_SPACE) && player_->GetIsFinish() == false)
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

	gameCamera_->Update(player_->GetIsMoving(),player_->GetTransform(),player_->GetInitPos(),player_->GetIsDead(),
	player_->GetDeadPos(),player_->GetTotalAxcell(),player_->GetOnGround());

	//カメラの切り替え
	player_->Update(gameCamera_.get());
	skydome_->Update(gameCamera_.get());
	plane_->Update(gameCamera_.get());
	backGround_->Update(gameCamera_.get());

	landParticle_->SetPlayerIsDead(player_->GetIsDead());
	deadParticle_->SetPlayerIsDead(player_->GetIsDead());
	breakParticle_->SetPlayerIsDead(player_->GetIsDead());

	if(gameSpeed_->GetSpeedMode() != GameSpeed::SpeedMode::STOP)
	{
		if(player_->GetOnGround() == false && player_->GetIsMoving() == true && player_->GetIsDead() == false)
		{
			if(player_->GetIsSecondJumpMoment() == true)
			{
				secondJumpParticle_->Preparation(player_->GetTransform(), player_->GetAttributeColor());
			}
		}

		if(player_->GetOnGround() == true && player_->GetIsMoving() == true && player_->GetIsDead() == false)
		{
			if(player_->GetIsSecondJumpMoment() == false)
			{
				groundParticle_->Preparation(player_->GetTransform(), player_->GetAttributeColor());
			}
		}
		landParticle_->PopUpdate(gameCamera_.get(), player_->GetTransform(), player_->GetIsLanded(), player_->GetAttributeColor());
		deadParticle_->PopUpdate(gameCamera_.get(), player_->GetTransform(), player_->GetIsDead(), player_->GetAttributeColor());
		breakParticle_->PopUpdate(gameCamera_.get(), stage_->GetBreakWallsPos());
	}

	ParticleManager::GetInstance()->Update(gameCamera_.get());
	

	stage_->Update(gameCamera_.get(), player_->GetRightAxcell());
	
	ObjParticleManager::GetInstance()->Update(gameCamera_.get());
	GameTimer::GetInstance()->InGameUpdate(player_->GetIsMoving(), player_->GetIsFinish());

#ifdef _DEBUG

	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();
	//secondJumpParticle_->ImGuiUpdate();
	breakParticle_->ImGuiUpdate();
	groundParticle_->ImguiUpdate();

#endif

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	if(player_->GetIsFinish() == true)
	{
		player_->SetIsMoving(false);
		gameCamera_->GoalAnimation();

		if(gameCamera_->GetIsFinishAnimation())
		{
			ParticleManager::GetInstance()->AllRemove();
			ObjParticleManager::GetInstance()->AllRemove();
			
			SoundManager::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("CLEAR");
		}
	}

#ifdef _DEBUG
	if(keys_->PushedKeyMoment(DIK_G))
	{
		player_->SetIsFinish(true);
		GameTimer::GetInstance()->InGameUpdate(player_->GetIsMoving(), player_->GetIsFinish());
		ParticleManager::GetInstance()->AllRemove();
		ObjParticleManager::GetInstance()->AllRemove();

		SoundManager::GetInstance()->Finalize();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
#endif
}

void GameScene::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();

	SpriteCommon::GetInstance()->BeforeDraw();
	backGroundSprite_->Draw("backGround.png");

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

	ObjParticleManager::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();

	SpriteCommon::GetInstance()->BeforeDraw();
	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");
	jumpSprite_->Draw("jump.png");
	arrowSprite_->Draw("arrow.png");
	GameTimer::GetInstance()->InGameDraw();
	sceneTransitionUp_->Draw("BlackBackGroundHalfTex");
	sceneTransitionDown_->Draw("BlackBackGroundHalfTex");
	
	//デバッグテキストの描画
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

void GameScene::SceneAnimation()
{
	if(sceneAnimeTimer_ < kSceneAnimeTime_)
	{
		sceneAnimeTimer_++;
	}
	else
	{
		sceneAnimeTimer_ = 0;
		isFinishAnimetion_ = true;
	}

	//変化量
	float x = sceneAnimeTimer_ / kSceneAnimeTime_;
	sceneAnimationVec_.y += easeOutQuint(x);

	//画像を動かす処理
	sceneTransitionUp_->MovePos(-sceneAnimationVec_);
	sceneTransitionDown_->MovePos(sceneAnimationVec_);
}

void GameScene::ResetSceneAnimation()
{
	if(isFinishAnimetion_ == true)
	{
		sceneTransitionUp_->SetPosition(Vector2(0.0f, 0.0f));
		sceneTransitionDown_->SetPosition(Vector2(0.0f, WindowsAPI::kWindow_height_ / 2));
		sceneAnimeTimer_ = 0;
		sceneAnimationVec_ = Vector2(0.0f, 0.0f);
		isFinishAnimetion_ = false;
	}
}

