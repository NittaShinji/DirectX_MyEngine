#include "MyGame.h"
#include "SceneFactory.h"
#include "StageSelect.h"
#include "ClearScene.h"

#include "Vector4.h"
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
	TitleScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());
	StageSelectScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	ClearScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
}

void MyGame::Update()
{
	// DirectX毎フレーム処理 ここから
	imGuiManager_->Begin();

	//基底クラスの更新処理
	MyFramework::Update();

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//ゲームシーンの描画
	SceneManager::GetInstance()->Draw();
}

void MyGame::Finalize()
{
	//ゲーム全体の終了処理
	imGuiManager_->Finalize();

	//基底クラスの終了処理
	MyFramework::Finalize();
}