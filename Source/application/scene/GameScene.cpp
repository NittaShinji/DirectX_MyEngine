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
KeyInput* GameScene::keys_ = nullptr;

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
	//3Dオブジェクトにライトをセット
	lightGroup_ = LightGroup::Create();
	Object3d::SetLightGroup(lightGroup_);
	const int32_t firstLight = 0;
	lightGroup_->SetDirLightActive(firstLight, true);

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
	gameSprite_ = std::make_unique<GameSprite>();
	gameSprite_->Initialize();

	//モデル読み込み
	Model::Load("Cube");
	Model::Load("wall");
	Model::Load("Plane");
	Model::Load("GoalWall");
	Model::Load("StageBlock");

	//3Dオブジェクトの生成
	player_ = Player::Create("sphere");
	player_->SetGamePad(gamePad_.get());
	mirrorPlayer_ = MirrorPlayer::Create(player_.get());

	stage_ = std::make_unique<Stage>();
	stage_->Initialize("Stage0.json", player_.get());

	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize("backGround.json");
	normalBackGround_ = std::make_unique<BackGround>();
	normalBackGround_->Initialize("normalOBJ.json");

	//------------カメラ----------
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();

	//2Dパーティクル
	ParticleManager::GetInstance()->Initialize();
	//3Dパーティクル
	ObjParticleManager::GetInstance()->Initialize();

	//ゲームスピード
	gameSpeed_ = std::make_unique<GameSpeed>();
	gameSpeed_->Initialize();
	player_->SetGameSpeed(gameSpeed_.get());
	gameCamera_->SetGameSpeed(gameSpeed_.get());

	//パーティクルにプレイヤー、ゲームスピード情報をセット
	ParticleManager::GetInstance()->SetEmitterPlayer(player_.get());
	ParticleManager::GetInstance()->SetEmitterGameSpeed(gameSpeed_.get());
	ObjParticleManager::GetInstance()->SetEmitterPlayer(player_.get());
	ObjParticleManager::GetInstance()->SetEmitterGameSpeed(gameSpeed_.get());

	//ゲームタイマー
	GameTimer::GetInstance()->InGameInitialize();
	//チュートリアルイベント
	tutorialEvent_ = std::make_unique<TutorialEvent>();
	Event::StaticInitialize(keys_, gamePad_.get(), gameSpeed_.get());
	tutorialEvent_->Initialzie(player_.get());
}

void GameScene::Update()
{
	//ゲームパッドを接続
	if(gamePad_->IsConnected(Player1)) {}

	//トランジションの更新
	gameSprite_->SceneAnimation();
	//ゲームスピードの更新
	gameSpeed_->Update();

#ifdef _DEBUG
	//ゲームスピードの切り替え
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

	//プレイヤーが死んだ際の処理
	if(player_->GetIsDead() == true || keys_->HasPushedKey(DIK_R))
	{
		ParticleManager::GetInstance()->ParticleRemove();
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);

		//死亡時パーティクルが消えてリセット可能かどうか
		bool canReset = ObjParticleManager::GetInstance()->GetDeadParticle()->GetCanReset();

		//リセット処理
		if(canReset == true || keys_->HasPushedKey(DIK_R))
		{
			ParticleManager::GetInstance()->ParticleRemove();
			ObjParticleManager::GetInstance()->ProcessPlayerDead(gameCamera_.get());

			stage_->Reset("Stage0.json");
			gameCamera_->Initialize();
			player_->Reset(gameCamera_.get());
			GameTimer::GetInstance()->Reset();
			gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
			gameSprite_->ResetSceneAnimation();
		}
	}

	//スプライト
	gameSprite_->Update();

	//ゲームが始まっていないときに始める処理
	if(player_->GetIsDead() == false && player_->GetIsFinish() == false)
	{
		if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A))
		{
			player_->SetIsMoving(true);
		}

		if(keys_->PushedKeyMoment(DIK_SPACE) )
		{
			player_->SetIsMoving(true);
		}
	}

	//光線方向初期値
	const Vector3 kLightDir = { 1,-1,-10 };
	const float kLightDirUp = 0.0f;
	const Vector3 kColor = { 1, 1, 1 };

	//ライトの設定
	lightGroup_->SetAmbientColor(kColor);
	lightGroup_->SetDirLightDir(0, kLightDir, kLightDirUp);
	const Vector3 whitelightColor = { 1.0f,1.0f,1.0f };
	lightGroup_->SetDirLightColor(0, whitelightColor);
	{
		//imguiからのライトパラメータを反映
		lightGroup_->SetAmbientColor(Vector3(ambientLightColor_));
		lightGroup_->SetDirLightDir(0, Vector3({ lightDir_.x + imGuiDir_[0], lightDir_.y + imGuiDir_[1], lightDir_.z + imGuiDir_[2] }), 0.0f);
		lightGroup_->SetDirLightColor(0, Vector3(lightColor_));
	}
	//ライトの更新
	lightGroup_->Update();

	//カメラの更新
	gameCamera_->Update(player_->GetIsMoving(),player_->GetTransform(),player_->GetInitPos(),player_->GetIsDead(),
	player_->GetDeadPos(),player_->GetRightAxcell());

	//カメラの切り替え
	player_->Update(gameCamera_.get());
	mirrorPlayer_->Update(gameCamera_.get());

	//背景オブジェクトの更新
	backGround_->Update(gameCamera_.get());
	normalBackGround_->Update(gameCamera_.get());
	tutorialEvent_->Update();
	stage_->Update(gameCamera_.get(), player_.get(),gameSpeed_.get());

	//パーティクルの生成準備
	ParticleManager::GetInstance()->Preparation(gameSpeed_.get(), player_.get());
	ObjParticleManager::GetInstance()->PopUpdate(gameSpeed_.get(),gameCamera_.get(),player_.get(),stage_.get());

	//パーティクルの更新
	ParticleManager::GetInstance()->Update(gameCamera_.get());
	ObjParticleManager::GetInstance()->Update(gameCamera_.get());

	//ゲームタイマーの更新
	GameTimer::GetInstance()->InGameUpdate(player_->GetIsMoving(), player_->GetIsFinish());

#ifdef _DEBUG

	//Imguiの更新
	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();
	ParticleManager::GetInstance()->ImGuiUpdate();
	stage_->ImguiUpdate();

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
	//デバッグ用
	//クリア画面に飛ぶ
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

	gameSprite_->BackGroundDraw();
	
	Object3d::BeforeDraw();
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

	//3Dオブジェクト描画
	Object3d::BeforeDraw();
	player_->Draw();

	//3Dパーティクル描画
	ObjParticleManager::GetInstance()->Draw();

	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	//2Dパーティクル描画
	ParticleManager::GetInstance()->Draw();
	
	//深度値クリア
	directXBasic_->ClearDepthBuffer();

	gameSprite_->UIDraw();
	GameTimer::GetInstance()->InGameDraw();
	gameSprite_->TransitionDraw();
	tutorialEvent_->Draw();
	
	//デバッグテキストの描画
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}
