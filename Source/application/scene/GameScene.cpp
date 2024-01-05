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
	const int32_t firstLight = 0;
	lightGroup_->SetDirLightActive(firstLight, true);
	//lightGroup_->SetDirLightActive(1, true);

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
	const int32_t kHalfWindowHeight = WindowsAPI::kWindow_height_ / 2;
	const Vector4 blackColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	//黒色のテクスチャ―を生成
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, kHalfWindowHeight, blackColor, "BlackBackGroundHalfTex");
	//画像読み込み
	TextureManager::GetInstance()->LoadTexture("jump.png");
	TextureManager::GetInstance()->LoadTexture("arrow.png");
	TextureManager::GetInstance()->LoadTexture("cloud.png");
	TextureManager::GetInstance()->LoadTexture("effect2.png");
	TextureManager::GetInstance()->LoadTexture("jumpEffect6.png");
	TextureManager::GetInstance()->LoadTexture("backGround.png");

	//aボタン画像
	aButtonSprite_ = std::make_unique<Sprite>();
	const Vector2 aButtonPosition = { 1152,648 };
	aButtonSprite_->Initialize("A.png",aButtonPosition);
	aButtonSprite_->SetSize(Vector2(kUiSize_));
	//bボタン画像
	bButtonSprite_ = std::make_unique<Sprite>();
	const Vector2 bButtonPosition = { 1216,648 };
	bButtonSprite_->Initialize("B.png",bButtonPosition);
	bButtonSprite_->SetSize(Vector2(kUiSize_));
	//ジャンプ画像
	jumpSprite_ = std::make_unique<Sprite>();
	const Vector2 jumpSpritePosition = { 1152,684 };
	jumpSprite_->Initialize("jump.png",jumpSpritePosition);
	jumpSprite_->SetSize(Vector2(kUiSize_));
	//矢印画像
	arrowSprite_ = std::make_unique<Sprite>();
	const Vector2 arrowPosition = { 1216,584 };
	arrowSprite_->Initialize("arrow.png",arrowPosition);
	arrowSprite_->SetSize(Vector2(kUiSize_));
	//背景画像
	backGroundSprite_ = std::make_unique<Sprite>();
	const Vector2 kDefaultSpritePos = { 0.0f,0.0f };
	backGroundSprite_->Initialize("backGround.png", kDefaultSpritePos);
	//トランジション用画像
	sceneTransitionUp_ = std::make_unique<Sprite>();
	sceneTransitionDown_ = std::make_unique<Sprite>();
	sceneTransitionUp_->Initialize("BlackBackGroundHalfTex", kDefaultSpritePos);
	sceneTransitionDown_->Initialize("BlackBackGroundHalfTex",Vector2(kDefaultSpritePos.x, kHalfWindowHeight));
	
	//モデル読み込み
	const string sphere = "sphere";
	const string test = "NoImageModel";
	const string testStage0 = "testStage0";
	const string ground = "ground";
	const string skydome = "skydome";
	const string cube = "Cube";
	const string plane = "Plane";
	const string wall = "wall";
	const string goalWall = "GoalWall";
	const string stageBlock = "StageBlock";

	Model::Load(testStage0);
	Model::Load(cube);
	Model::Load(wall);
	Model::Load(plane);
	Model::Load(goalWall);
	Model::Load(stageBlock);

	//3Dオブジェクトの生成
	stage_ = std::make_unique<Stage>();
	stage_->Initialize("Stage0.json");

	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize("backGround.json");
	normalBackGround_ = std::make_unique<BackGround>();
	normalBackGround_->Initialize("normalOBJ.json");

	player_ = Player::Create(sphere);
	player_->SetGamePad(gamePad_.get());
	mirrorPlayer_ = MirrorPlayer::Create(player_.get());

	//------------カメラ----------
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();

	//2Dパーティクル
	groundParticle_ = GroundParticle::Create("cloud.png");
	hitParticle_ = HitParticle2D::Create("cloud.png");
	secondJumpParticle_ = SecondJump2DParticle::Create("jumpEffect6.png");
	ParticleManager::GetInstance()->AddEmitter(groundParticle_.get());
	ParticleManager::GetInstance()->AddEmitter(secondJumpParticle_.get());
	ParticleManager::GetInstance()->AddEmitter(hitParticle_.get());
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
	hitParticle_->SetGameSpeed(gameSpeed_.get());

	landParticle_->SetGameSpeed(gameSpeed_.get());
	deadParticle_->SetGameSpeed(gameSpeed_.get());
	breakParticle_->SetGameSpeed(gameSpeed_.get());

	GameTimer::GetInstance()->InGameInitialize();

	
	isReset_ = false;
}

void GameScene::Update()
{
	SceneAnimation();
	gameSpeed_->Update();

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

	if(player_->GetIsDead() == true || keys_->HasPushedKey(DIK_R))
	{
		if(isReset_ == false)
		{
			isReset_ = true;
		}

		ParticleManager::GetInstance()->ParticleRemove();

		if(deadParticle_->GetCanReset() == true || keys_->HasPushedKey(DIK_R))
		{
			ObjParticleManager::GetInstance()->ParticleReset(gameCamera_.get());
			stage_->Reset("Stage0.json");
			gameCamera_->Initialize();
			player_->Reset(gameCamera_.get());
			isReset_ = false;
			deadParticle_->SetCanReset(false);
			deadParticle_->Reset();
			GameTimer::GetInstance()->Reset();
			gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
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
	const Vector3 whitelightColor = { 1.0f,1.0f,1.0f };
	lightGroup_->SetDirLightColor(0, whitelightColor);
	
	{
		//imguiからのライトパラメータを反映
		lightGroup_->SetAmbientColor(Vector3(ambientColor0_));
		lightGroup_->SetDirLightDir(0, Vector3({ lightDir0_.x + imGuiDir_[0], lightDir0_.y + imGuiDir_[1], lightDir0_.z + imGuiDir_[2] }), 0.0f);
		lightGroup_->SetDirLightColor(0, Vector3(lightColor0_));
	}

	lightGroup_->Update();

	gameCamera_->Update(player_->GetIsMoving(),player_->GetTransform(),player_->GetInitPos(),player_->GetIsDead(),
	player_->GetDeadPos(),player_->GetRightAxcell());

	//カメラの切り替え
	player_->Update(gameCamera_.get());
	mirrorPlayer_->Update(gameCamera_.get());

	backGround_->Update(gameCamera_.get());
	normalBackGround_->Update(gameCamera_.get());

	landParticle_->SetPlayerIsDead(player_->GetIsDead());
	deadParticle_->SetPlayerIsDead(player_->GetIsDead());
	breakParticle_->SetPlayerIsDead(player_->GetIsDead());
	groundParticle_->SetIsPlayerAxcelled(player_->GetRightAxcell());

	if(player_->GetIsLanded() == true)
	{
		groundParticle_->SetIsPlayerColor(player_->GetAttributeColor());
	}
	
	if(gameSpeed_->GetSpeedMode() != GameSpeed::SpeedMode::STOP)
	{
		if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::SLOW)
		{
			if(gameSpeed_->GetCanMoveInSlow() == true)
			{
				if(player_->GetOnGround() == false && player_->GetIsMoving() == true && player_->GetIsDead() == false)
				{
					if(player_->GetIsSecondJumpMoment() == true)
					{
						secondJumpParticle_->Preparation(player_->GetTransform());
					}
				}

				if(player_->GetOnGround() == true && player_->GetIsMoving() == true && player_->GetIsDead() == false)
				{
					if(player_->GetIsSecondJumpMoment() == false)
					{
						groundParticle_->Preparation(player_->GetTransform(), player_->GetAttributeColor(), player_->GetRightAxcell(),gameSpeed_->GetCanMoveInSlow());
					}
				}

				if(player_->GetIsMoving() == true && player_->GetIsDead() == false)
				{
					bool isTouchObject = player_->GetIsTouchObject();

					if(player_->GetIsLanded() == true || isTouchObject == true)
					{
						hitParticle_->Preparation(player_->GetTransform());
						if(isTouchObject == true)
						{
							player_->SetIsTouchObject(false);
						}
					}
				}

				landParticle_->PopUpdate(gameCamera_.get(), player_->GetTransform(), player_->GetIsLanded(), player_->GetAttributeColor());
				deadParticle_->PopUpdate(gameCamera_.get(), player_->GetTransform(), player_->GetIsDead(), player_->GetAttributeColor());
				breakParticle_->PopUpdate(gameCamera_.get(), stage_->GetBreakWallsPos());
			}
		}
		else
		{
			if(player_->GetOnGround() == false && player_->GetIsMoving() == true && player_->GetIsDead() == false)
			{
				if(player_->GetIsSecondJumpMoment() == true)
				{
					secondJumpParticle_->Preparation(player_->GetTransform());
				}
			}

			if(player_->GetOnGround() == true && player_->GetIsMoving() == true && player_->GetIsDead() == false)
			{
				if(player_->GetIsSecondJumpMoment() == false)
				{
					groundParticle_->Preparation(player_->GetTransform(), player_->GetAttributeColor(), player_->GetRightAxcell(), gameSpeed_->GetCanMoveInSlow());
				}
			}

			if(player_->GetIsMoving() == true && player_->GetIsDead() == false)
			{
				bool isTouchObject = player_->GetIsTouchObject();

				if(player_->GetIsLanded() == true || isTouchObject == true)
				{
					hitParticle_->Preparation(player_->GetTransform());
					if(isTouchObject == true)
					{
						player_->SetIsTouchObject(false);
					}
				}
			}

			landParticle_->PopUpdate(gameCamera_.get(), player_->GetTransform(), player_->GetIsLanded(), player_->GetAttributeColor());
			deadParticle_->PopUpdate(gameCamera_.get(), player_->GetTransform(), player_->GetIsDead(), player_->GetAttributeColor());
			breakParticle_->PopUpdate(gameCamera_.get(), stage_->GetBreakWallsPos());
		}
	}	

	stage_->Update(gameCamera_.get(), player_.get());

	ParticleManager::GetInstance()->Update(gameCamera_.get());
	ObjParticleManager::GetInstance()->Update(gameCamera_.get());
	GameTimer::GetInstance()->InGameUpdate(player_->GetIsMoving(), player_->GetIsFinish());

#ifdef _DEBUG

	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();
	breakParticle_->ImGuiUpdate();
	groundParticle_->ImguiUpdate();

	ImGui::Begin("light");
	const Vector2 kImGuiPos = { 0.0f,360.0f };
	const Vector2 kImGuiSize = { 200.0f,100.0f };

	ImGui::SetWindowPos(ImVec2(kImGuiPos.x,kImGuiPos.y));
	ImGui::SetWindowSize(ImVec2(kImGuiSize.x, kImGuiSize.y));

	const Vector2 kImGuiRate = { -30.0f,30.0f };

	ImGui::SliderFloat3("lightDir", imGuiDir_, kImGuiRate.x, kImGuiRate.y);
	
	ImGui::End();

#endif

	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();

	//ゴールに近づいたらスローにする
	if(stage_->GetGoal()->GetIsStartGoalStagin() == true)
	{
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::SLOW);
	}

	//ゴールに触れたら
	if(player_->GetIsFinish() == true)
	{
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
		stage_->GetGoal()->SetIsStartGoalStagin(false);
		player_->SetIsRightAxcell(true);
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
	//plane_->Draw();
	backGround_->Draw();
	mirrorPlayer_->Draw();

	postEffect_->PostDrawScene();

	//描画開始
	directXBasic_->BeforeDraw();
	
	//ポストエフェクトの描画
	postEffect_->Draw();

	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	//モデル描画
	Object3d::BeforeDraw();
	normalBackGround_->Draw();
	stage_->Draw();
	
	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	ParticleManager::GetInstance()->Draw();
	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	Object3d::BeforeDraw();
	player_->Draw();
	
	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	ObjParticleManager::GetInstance()->Draw();
	
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
	sceneAnimationVec_.y += EaseOutQuint(x);

	//画像を動かす処理
	sceneTransitionUp_->MovePos(-sceneAnimationVec_);
	sceneTransitionDown_->MovePos(sceneAnimationVec_);
}

void GameScene::ResetSceneAnimation()
{
	if(isFinishAnimetion_ == true)
	{
		sceneTransitionUp_->SetPosition(Vector2(0.0f, 0.0f));
		const float kHalfWindowHeight = WindowsAPI::kWindow_height_ / 2;
		sceneTransitionDown_->SetPosition(Vector2(0.0f, kHalfWindowHeight));
		sceneAnimeTimer_ = 0;
		sceneAnimationVec_ = Vector2(0.0f, 0.0f);
		isFinishAnimetion_ = false;
	}
}

