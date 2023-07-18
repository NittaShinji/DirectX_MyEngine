#include "MyGame.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

MyGame::MyGame(){}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	MyFramework::Initialize();

	//ImGui初期化処理
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), directXBasic_.get());

	//GameScene初期化処理
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(directXBasic_.get(), imGuiManager_.get());

	//ポストエフェクト初期化処理
	postEffect_ = std::make_unique<PostEffect>();
	//ポストエフェクト用テクスチャの読み込み
	SpriteCommon::LoadTexture("test.png");
	postEffect_->Initialize(directXBasic_.get());
}

void MyGame::Update()
{
	//基底クラスの更新処理
	MyFramework::Update();

	postEffect_->SetAnchorPoint(XMFLOAT2(0.0f, 0.0f));

	// DirectX毎フレーム処理 ここから
	imGuiManager_->Begin();

	gameScene_->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();
	gameScene_->Draw();
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