#include "StageSelect.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"
#include "TextureManager.h"

DirectXBasic* StageSelectScene::directXBasic_ = nullptr;
ImGuiManager* StageSelectScene::imGuiManager_ = nullptr;

void StageSelectScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	TextureManager::StaticInitialize(directXBasic_->GetDevice().Get());
	Sprite::StaticInitialize();
}

void StageSelectScene::Initialize()
{
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//画像
	selectSprite_ = std::make_unique<Sprite>();
	BackGroundLeft_ = std::make_unique<Sprite>();
	BackGroundRight_ = std::make_unique<Sprite>();
	BackGroundWhite_ = std::make_unique<Sprite>();
	gameSceneSprite1_ = std::make_unique<Sprite>();
	gameSceneSprite2_ = std::make_unique<Sprite>();

	const int32_t selectWidth = 640;
	const int32_t selectHeight = 400;
	const Vector2 selectSize = { selectWidth,selectHeight };
	TextureManager::GetInstance()->TexMapping(selectWidth, selectHeight, Vector4(0.8f, 0.8f, 0.8f, 1.0f), "CursorTex");
	//灰色のテクスチャ―
	TextureManager::GetInstance()->TexMapping(600, 360, Vector4(0.746f, 0.746f, 0.746f, 1.0f), "BackGroundTex");

	TextureManager::GetInstance()->LoadTexture("GameScene1.png");
	TextureManager::GetInstance()->LoadTexture("GameScene2.png");

	Vector2 selectPosition = { 0.0f,160.0f };
	selectSprite_->Initialize(selectPosition, selectSize);

	Vector2 testPosition = { 0.0f,0.0f };
	const Vector2 testSize = { 128.0f,128.0f };

	Vector2 backGroundPositionL = { 20.0f,180.0f };
	Vector2 backGroundPositionR = { 660.0f,180.0f };
	const Vector2 backGroundSize = { 600.0f,360.0f };
	BackGroundLeft_->Initialize(backGroundPositionL, backGroundSize);
	BackGroundRight_->Initialize(backGroundPositionR, backGroundSize);
	BackGroundWhite_->Initialize(Vector2(0.0f, 0.0f), Vector2(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_));
	gameSceneSprite1_->Initialize(backGroundPositionL, backGroundSize);
	gameSceneSprite2_->Initialize(backGroundPositionR, backGroundSize);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	//Sound::GetInstance()->LoadSoundWave("title.wav");
	//Sound::GetInstance()->LoadSoundWave("touch.wav");
	//Sound::GetInstance()->PlaySoundWave("title.wav",false);
}

void StageSelectScene::Update()
{
	//画像の更新
	selectSprite_->matUpdate();
	BackGroundLeft_->matUpdate();
	BackGroundRight_->matUpdate();
	BackGroundWhite_->matUpdate();
	gameSceneSprite1_->matUpdate();
	gameSceneSprite2_->matUpdate();


	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();

	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		//Sound::GetInstance()->PlaySoundWave("touch.wav",false);
		isChangeScene_ = true;
	}

	if(keys_->PushedKeyMoment(DIK_RIGHT))
	{
		if(selectSprite_->GetPosition().x == 0.0f)
		{
			selectSprite_->SetPosition(Vector2(WindowsAPI::kWindow_width_ / 2, 160.0));
		}
	}
	else if(keys_->PushedKeyMoment(DIK_LEFT))
	{
		if(selectSprite_->GetPosition().x == WindowsAPI::kWindow_width_ / 2)
		{
			selectSprite_->SetPosition(Vector2(0.0f, 160.0f));
		}
	}

	if(isChangeScene_ == true)
	{
		changeSceneTimer_--;
		if(changeSceneTimer_ <= 0)
		{
			//Sound::GetInstance()->Finalize();
			//SceneManager::GetInstance()->ChangeScene("Tutorial");
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}

void StageSelectScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	BackGroundWhite_->Update();
	selectSprite_->Update();
	BackGroundLeft_->Update();
	BackGroundRight_->Update();
	gameSceneSprite1_->Update();
	gameSceneSprite2_->Update();

	BackGroundWhite_->Draw("WhiteTex");
	selectSprite_->Draw("CursorTex");
	BackGroundLeft_->Draw("BackGroundTex");
	BackGroundRight_->Draw("BackGroundTex");
	gameSceneSprite1_->Draw("GameScene1.png");
	gameSceneSprite2_->Draw("GameScene2.png");

	//描画終了
	directXBasic_->AfterDraw();

}