#include "MyGame.h"
#include "SceneFactory.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

MyGame::MyGame(){}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	MyFramework::Initialize();
	
	//ゲームシーン初期化処理
	//シーンファクトリーを生成し、マネージャにセット
	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(std::move(sceneFactory_));
	//シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
	//一度のみ初期化
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());

	//ポストエフェクト初期化処理
	postEffect_ = std::make_unique<PostEffect>();
	//ポストエフェクト用テクスチャの読み込み
	SpriteCommon::GetInstance()->LoadTexture("test.png");
	postEffect_->Initialize(directXBasic_.get());

	//パーティクル初期化処理
	//particleManager_ = std::make_unique<ParticleManager>();
}

void MyGame::Update()
{
	// DirectX毎フレーム処理 ここから
	imGuiManager_->Begin();

	//基底クラスの更新処理
	MyFramework::Update();

	postEffect_->SetAnchorPoint(XMFLOAT2(0.0f, 0.0f));

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();
	SceneManager::GetInstance()->Draw();
	postEffect_->PostDrawScene();

	//描画開始
	directXBasic_->BeforeDraw();
	//ポストエフェクトの描画
	postEffect_->Draw("test.png");
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

void MyGame::Finalize()
{
	//ゲーム全体の終了処理
	imGuiManager_->Finalize();

	//基底クラスの終了処理
	MyFramework::Finalize();
}