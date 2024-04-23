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
using namespace NsEngine;

DirectXBasic* GameScene::directXBasic_ = nullptr;
ImGuiManager* GameScene::imGuiManager_ = nullptr;
KeyInput* GameScene::keys_ = nullptr;
bool GameScene::isClearGame_ = false;

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	Billboard::StaticInitialize(directXBasic_->GetDevice().Get(), directXBasic_->GetCommandList().Get());
	ParticleManager::GetInstance()->StaticInitialize(directXBasic_->GetDevice().Get(), directXBasic_->GetCommandList().Get());
	ObjParticleManager::GetInstance()->StaticInitialize(directXBasic_->GetDevice().Get(), directXBasic_->GetCommandList().Get());
	ResultSprite::StaticInitialize();
}

void GameScene::Initialize()
{
	//サウンド
	gameSound_ = SoundManager::GetInstance()->GetSound("gamesceneBGM.wav");
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
	resultSprite_ = std::make_unique<ResultSprite>();
	resultSprite_->Initialize();

	//ライトの初期化処理
	//3Dオブジェクトにライトをセット
	lightGroup_ = LightGroup::Create();
	Object3d::SetLightGroup(lightGroup_);
	//ライトを有効化し、光線方向の初期値設定
	const int32_t firstLight = 0;
	lightGroup_->SetDirLightActive(firstLight, true);
	lightGroup_->SetAmbientColor(Vector3(ambientLightColor_));
	lightGroup_->SetDirLightDir(0, Vector3({ lightDir_.x, lightDir_.y, lightDir_.z }), 0.0f);
	lightGroup_->SetDirLightColor(0, Vector3(lightColor_));
	//設定したライト情報を更新
	lightGroup_->Update();

	//モデル読み込み
	Model::Load("Cube");
	Model::Load("Wall");
	Model::Load("GoalWall");
	Model::Load("StageBlock");

	//3Dオブジェクトの生成
	player_ = Player::Create("Sphere");
	player_->SetGamePad(gamePad_.get());
	mirrorPlayer_ = MirrorPlayer::Create(player_.get());

	stage_ = std::make_unique<Stage>();
	stage_->Initialize(player_.get());

	//カメラ
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();

	//ゲームスピード
	gameSpeed_ = std::make_unique<GameSpeed>();
	gameSpeed_->Initialize();
	player_->SetGameSpeed(gameSpeed_.get());
	gameCamera_->SetGameSpeed(gameSpeed_.get());

	//2Dパーティクル
	ParticleManager::GetInstance()->Initialize();
	//3Dパーティクル
	ObjParticleManager::GetInstance()->Initialize();

	//パーティクルにプレイヤー、ゲームスピード情報をセット
	ParticleManager::GetInstance()->SetEmitterPlayer(player_.get());
	ParticleManager::GetInstance()->SetEmitterGameSpeed(gameSpeed_.get());
	ObjParticleManager::GetInstance()->SetEmitterPlayer(player_.get());
	ObjParticleManager::GetInstance()->SetEmitterGameSpeed(gameSpeed_.get());

	//ゲームタイマー
	GameTimer::GetInstance()->InGameInitialize();
	GameTimer::GetInstance()->ResultInitialize(resultSprite_->GetBackGroundSpritePosY());

	//チュートリアルイベント
	tutorialEvent_ = std::make_unique<TutorialEvent>();
	Event::StaticInitialize(keys_, gamePad_.get(), gameSpeed_.get());
	if (isClearGame_ == false)
	{
		tutorialEvent_->Initialzie(player_.get(), gameCamera_.get());
	}
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

	//imguiからのライトパラメータを反映
	lightGroup_->SetDirLightDir(0, Vector3({ lightDir_.x + imGuiDir_[0], lightDir_.y + imGuiDir_[1], lightDir_.z + imGuiDir_[2] }), 0.0f);
	lightGroup_->Update();

#endif

	if(stage_->GetStageNum() == Stage::tutorialStage && isClearGame_ == false)
	{
		tutorialEvent_->Update();
	}
	
	//プレイヤーが死んだ際の処理
	if(player_->GetIsDead() == true || keys_->HasPushedKey(DIK_R))
	{
		GameTimer::GetInstance()->SetIsTImed(false);
		gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);

		//死亡時パーティクルが消えてリセット可能かどうか
		bool canReset = ObjParticleManager::GetInstance()->GetDeadParticle()->GetCanReset();

		//リセット処理
		if(canReset == true || keys_->HasPushedKey(DIK_R))
		{
			ParticleManager::GetInstance()->ParticleRemove();
			ObjParticleManager::GetInstance()->ProcessPlayerDead(gameCamera_.get());

			stage_->Reset();
			gameCamera_->Initialize();
 			player_->Reset();
			resultSprite_->Reset();
			if (stage_->GetStageNum() == 0 && isClearGame_ == false)
			{
				tutorialEvent_->ResetEventforSavePoint();
			}
			else
			{
				GameTimer::GetInstance()->Reset();
			}
			gameSpeed_->SetSpeedMode(GameSpeed::SpeedMode::NORMAL);
			gameSprite_->ResetSceneAnimation();
		}
	}

	//次のステージの端に来たらタイマーをスタートさせる
	//プレイヤーがステージの端にいるかどうかの状態をboolなどで判断
	if(player_->GetIsMoving() == true && player_->GetIsFinish() == false && player_->GetIsWithStageEdge() == true)
	{
		GameTimer::GetInstance()->SetIsTImed(true);
	}

	//スプライト
	gameSprite_->Update();

	//カメラの更新
	gameCamera_->Update(player_->GetIsMoving(), player_->GetTransform(), player_->GetInitPos(), player_->GetIsDead(),
		player_->GetDeadPos(), player_->GetRightAxcell(),player_->GetTotalAxcell());

	//カメラの切り替え
	player_->Update(gameCamera_.get());
	mirrorPlayer_->Update(gameCamera_.get());

	//背景オブジェクトの更新
	stage_->Update(gameCamera_.get(), player_.get(), gameSpeed_.get());

	//パーティクルの生成準備
	ParticleManager::GetInstance()->Preparation(gameSpeed_.get(), player_.get());
	ObjParticleManager::GetInstance()->PopUpdate(gameSpeed_.get(), gameCamera_.get(), player_.get(), stage_.get());

	//パーティクルの更新
	ParticleManager::GetInstance()->Update(gameCamera_.get());
	ObjParticleManager::GetInstance()->Update(gameCamera_.get());

	//ゲームタイマーの更新
	GameTimer::GetInstance()->InGameUpdate(player_->GetIsMoving(), player_->GetIsFinish(),stage_->GetIsPlayerReachedStageEdge());
	GameTimer::GetInstance()->HighScoreSpriteUpdate();
	GameTimer::GetInstance()->ResultUpdate(resultSprite_->GetIsFinishInEasing(), resultSprite_->GetBackGroundSpritePosY(),stage_->GetStageNum(),stage_->GetKClearStageNum());

	SoundManager::GetInstance()->Update();

#ifdef _DEBUG

	//Imguiの更新
	gameCamera_->ImGuiUpdate();
	player_->ImGuiUpdate();
	ParticleManager::GetInstance()->ImGuiUpdate();
	stage_->ImguiUpdate();

	ImGui::Begin("light");
	const Vector2 kImGuiPos = { 0.0f,360.0f };
	const Vector2 kImGuiSize = { 200.0f,100.0f };

	ImGui::SetWindowPos(ImVec2(kImGuiPos.x, kImGuiPos.y));
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
		player_->SetIsWithStageEdge(false);
		GameTimer::GetInstance()->SetIsTImed(false);
		//ゴールスロー演出を辞める
		stage_->GetGoal()->SetIsStartGoalStagin(false);
		//スコアボードを画面内に入れる
		resultSprite_->ComeInScreen();

		//ステージをすべてクリアしていたらクリアシーンに遷移
		if(stage_->GetIsClearedAllStage() == true)
		{
			ClearOnceUpdate();
		}
		else
		{
			//一度だけステージ数を増やす
			stage_->NextStageUpdate();
			stage_->SetIsAllowedToCountStageNum(false);
			GameTimer::GetInstance()->CalculateHighTime(stage_->GetStageNum(), stage_->GetKClearStageNum());

			//Aボタンが押されたら次のステージを生成
			if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A) || keys_->PushedKeyMoment(DIK_SPACE))
			{
				player_->SetIsFinish(false);
				resultSprite_->SetIsFinishOutEasing(false);
				stage_->NextStageLoad();
				GameTimer::GetInstance()->ReadytoNextStageTime(stage_->GetStageNum(), stage_->GetKClearStageNum());
				GameTimer::GetInstance()->Reset();
			}
		}
	}
	else
	{
		//クリア状態でなければ場外にスコアボードは画面外出しておく
		resultSprite_->ComeOutOffScreen();
		stage_->SetIsAllowedToCountStageNum(true);

		//もしクリアしていたらクリアシーンに遷移
		if (stage_->GetStageNum() == stage_->GetKClearStageNum())
		{
			ClearOnceUpdate();
		}
	}

	//リザルト画面の更新
	resultSprite_->Update();

#ifdef _DEBUG
	//デバッグ用
	//クリア画面に飛ぶ
	if(keys_->PushedKeyMoment(DIK_H))
	{
		player_->SetIsFinish(true);
		GameTimer::GetInstance()->ReadytoNextStageTime(stage_->GetStageNum(), stage_->GetKClearStageNum());
		GameTimer::GetInstance()->InGameUpdate(player_->GetIsMoving(), player_->GetIsFinish(), stage_->GetIsPlayerReachedStageEdge());
		ParticleManager::GetInstance()->AllRemove();
		ObjParticleManager::GetInstance()->AllRemove();
		SoundManager::GetInstance()->StopAllSound();
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
	mirrorPlayer_->Draw();
	stage_->MirrorDraw();
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

	//チュートリアルステージであれば
	if(stage_->GetStageNum() == Stage::tutorialStage && isClearGame_ == false)
	{
		tutorialEvent_->Draw();
	}

	if (player_->GetIsFinish() == false)
	{
		//左上のカウントを描画する
		GameTimer::GetInstance()->InGameDraw();
		GameTimer::GetInstance()->HighScoreDraw();
	}

	resultSprite_->Draw();
	GameTimer::GetInstance()->ResultDraw();

	gameSprite_->TransitionDraw();

	//デバッグテキストの描画
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

void GameScene::ClearOnceUpdate()
{
	if (resultSprite_->GetIsFinishInEasing() == true)
	{
		if (GameTimer::GetInstance()->GetIsFinishedToTime() == true)
		{
			isClearGame_ = true;
			GameTimer::GetInstance()->ReadytoNextStageTime(stage_->GetStageNum(), stage_->GetKClearStageNum());
			ParticleManager::GetInstance()->AllRemove();
			ObjParticleManager::GetInstance()->AllRemove();
			ResultRoopStage::SetIsFinishedRoopObjects(false);
			SoundManager::GetInstance()->StopAllSound();
			SceneManager::GetInstance()->ChangeScene("CLEAR");
		}
	}
}
