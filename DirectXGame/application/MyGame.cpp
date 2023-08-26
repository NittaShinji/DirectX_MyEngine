#include "MyGame.h"
#include "SceneFactory.h"
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
	SceneManager::GetInstance()->ChangeScene("GAME");
	//一度のみ初期化
	TitleScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());

	//ポストエフェクト初期化処理
	postEffect_ = std::make_unique<PostEffect>();
	
	postEffect_->Initialize(directXBasic_.get());

	//背景オブジェクトのテスト用に仮カメラを配置
	Vector3 cameraEye = { 30,7.5,-20 };
	Vector3 cameraTarget = { 0,5,5 };
	Vector3 cameraUp = { 0,1,0 };
	testCamera_ = std::make_unique<Camera>();
	testCamera_->Initialize(cameraEye, cameraTarget, cameraUp);

	//ブラー用の背景オブジェクト
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();
}

void MyGame::Update()
{
	// DirectX毎フレーム処理 ここから
	imGuiManager_->Begin();

	//基底クラスの更新処理
	MyFramework::Update();

	postEffect_->SetAnchorPoint(Vector2(0.0f, 0.0f));

	//カメラ、背景オブジェクトの更新
	testCamera_->Update();
	backGround_->Update(testCamera_.get());

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
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
	
	//ゲームシーンの描画
	SceneManager::GetInstance()->Draw();

	//デバッグテキストの描画
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