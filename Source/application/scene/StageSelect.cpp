#include "StageSelect.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"
#include "TextureManager.h"

using namespace NsEngine;

DirectXBasic* StageSelectScene::directXBasic_ = nullptr;
ImGuiManager* StageSelectScene::imGuiManager_ = nullptr;

void StageSelectScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	const int32_t selectWidth = 640;
	const int32_t selectHeight = 400;

	const Vector4 grayColor = { 0.8f, 0.8f, 0.8f, 1.0f };
	const Vector4 blackColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	//灰色のテクスチャ―
	TextureManager::GetInstance()->TexMapping(selectWidth, selectHeight, grayColor, "CursorTex");
	//黒色のテクスチャ―
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, blackColor, "BlackBackGroundTex");
}

void StageSelectScene::Initialize()
{
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//画像
	selectSprite_ = std::make_unique<Sprite>();
	backGroundLeft_ = std::make_unique<Sprite>();
	backGroundRight_ = std::make_unique<Sprite>();
	backGroundWhite_ = std::make_unique<Sprite>();
	gameSceneSprite1_ = std::make_unique<Sprite>();
	gameSceneSprite2_ = std::make_unique<Sprite>();
	nowLoadingSprite_ = std::make_unique<Sprite>();

	TextureManager::GetInstance()->LoadTexture("GameScene1.png");
	TextureManager::GetInstance()->LoadTexture("GameScene2.png");
	TextureManager::GetInstance()->LoadTexture("nowLoading.png");


	const Vector2 selectPosition = { 0.0f,160.0f };
	
	const Vector2 testPosition = { 0.0f,0.0f };

	const Vector2 backGroundPositionL = { 20.0f,180.0f };
	const Vector2 backGroundPositionR = { 660.0f,180.0f };

	const Vector2 InitSpritePos = { 0.0f,0.0f };
	backGroundWhite_->Initialize("WhiteTex", InitSpritePos);
	selectSprite_->Initialize("CursorTex", selectPosition);
	gameSceneSprite1_->Initialize("GameScene1.png",backGroundPositionL);
	gameSceneSprite2_->Initialize("GameScene2.png",backGroundPositionR);
	nowLoadingSprite_->Initialize("nowLoading.png", InitSpritePos);
	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	//SoundManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->LoadSoundWave("title.wav");
	SoundManager::GetInstance()->LoadSoundWave("touch.wav");
	bgmSound_ = std::make_unique<Sound>();
	touchSound_ = std::make_unique<Sound>();
	bgmSound_->Initialize("title.wav");
	touchSound_->Initialize("touch.wav");
	bgmSound_->PlaySoundWave(true);
}

void StageSelectScene::Update()
{
	//画像の更新
	selectSprite_->matUpdate();
	backGroundWhite_->matUpdate();
	gameSceneSprite1_->matUpdate();
	gameSceneSprite2_->matUpdate();
	nowLoadingSprite_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}

	if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A) || keys_->PushedKeyMoment(DIK_RETURN))
	{
		touchSound_->PlaySoundWave(false);
		isChangeScene_ = true;
	}

	//カーソルのスプライトを移動させる
	const float kWindowHalfX = WindowsAPI::kWindow_width_ / 2;
	const float kSelectSpritePos = 160.0f;


	if(keys_->PushedKeyMoment(DIK_RIGHT) || gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		if(selectSprite_->GetPosition().x == 0.0f)
		{
			selectSprite_->SetPosition(Vector2(kWindowHalfX, kSelectSpritePos));
		}
	}
	else if(keys_->PushedKeyMoment(DIK_LEFT) || gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		if(selectSprite_->GetPosition().x == kWindowHalfX)
		{
			selectSprite_->SetPosition(Vector2(0.0f, kSelectSpritePos));
		}
	}

	if(isChangeScene_ == true)
	{
		
		changeSceneTimer_--;
		if(changeSceneTimer_ <= 0)
		{
			//SoundManager::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}

void StageSelectScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	backGroundWhite_->Draw("WhiteTex");
	selectSprite_->Draw("CursorTex");
	gameSceneSprite1_->Draw("GameScene1.png");
	gameSceneSprite2_->Draw("GameScene2.png");

	if(isChangeScene_ == true)
	{
		nowLoadingSprite_->Draw("nowLoading.png");
	}
	
	//描画終了
	directXBasic_->AfterDraw();

}