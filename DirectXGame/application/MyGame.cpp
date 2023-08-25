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
	SceneManager::GetInstance()->ChangeScene("TITLE");
	//一度のみ初期化
	TitleScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());

	//ポストエフェクト初期化処理
	postEffect_ = std::make_unique<PostEffect>();
	//ポストエフェクト用テクスチャの読み込み
	//SpriteCommon::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), "RedTex");
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), "RedTex");

	//SpriteCommon::GetInstance()->LoadTexture("postEffect.png");
	postEffect_->Initialize(directXBasic_.get());

	Vector3 cameraEye = { 30,7.5,-20 };

	Vector3 testCameraEye = { 30,7.5,-20 };
	Vector3 cameraTarget = { 0,5,5 };

	Vector3 testcameraTarget = { 0,5,5 };

	Vector3 cameraUp = { 0,1,0 };

	lightGroup_ = LightGroup::Create();
	//3Dオブジェクトにライトをセット
	Object3d::SetLightGroup(lightGroup_);

	testCamera_ = std::make_unique<Camera>();
	testCamera_->Initialize(cameraEye, cameraTarget, cameraUp);

	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	/*backGroundSprite_ = std::make_unique<Sprite>();
	TextureManager::GetInstance()->LoadTexture("TitleFont.png");

	Vector2 backGroundPosition = { 0.0f,0.0f };
	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;
	const Vector2 backGroundSize = { backGroundWidth,backGroundHeight };

	backGroundSprite_->Initialize(backGroundPosition, backGroundSize);
	*/
}

void MyGame::Update()
{
	// DirectX毎フレーム処理 ここから
	imGuiManager_->Begin();

	//基底クラスの更新処理
	MyFramework::Update();

	postEffect_->SetAnchorPoint(Vector2(0.0f, 0.0f));

	//backGroundSprite_->matUpdate();
	testCamera_->Update();
	backGround_->Update(testCamera_.get());

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	/*SpriteCommon::GetInstance()->BeforeDraw();
	backGroundSprite_->Update();
	backGroundSprite_->Draw("TitleFont.png");*/

	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();
	//モデル描画
	/*Object3d::BeforeDraw();
	backGround_->Draw();*/
	SceneManager::GetInstance()->Draw();

	postEffect_->PostDrawScene();

	//描画開始
	directXBasic_->BeforeDraw();

	//ポストエフェクトの描画
	postEffect_->Draw("RedTex");
	
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