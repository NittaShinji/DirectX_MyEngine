#include "StageSelect.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"
#include "TextureManager.h"

DirectXBasic* StageSelectScene::directXBasic_ = nullptr;
ImGuiManager* StageSelectScene::imGuiManager_ = nullptr;

void StageSelectScene::StaticInitialize()
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
	backGroundLeft_ = std::make_unique<Sprite>();
	backGroundRight_ = std::make_unique<Sprite>();
	backGroundWhite_ = std::make_unique<Sprite>();
	gameSceneSprite1_ = std::make_unique<Sprite>();
	gameSceneSprite2_ = std::make_unique<Sprite>();
	//sceneTransition_ = std::make_unique<Sprite>();
	nowLoadingSprite_ = std::make_unique<Sprite>();


	const int32_t selectWidth = 640;
	const int32_t selectHeight = 400;

	const Vector4 grayColor = { 0.8f, 0.8f, 0.8f, 1.0f };
	const Vector4 blackColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	//灰色のテクスチャ―
	TextureManager::GetInstance()->TexMapping(selectWidth, selectHeight, grayColor, "CursorTex");
	//TextureManager::GetInstance()->TexMapping(600, 360, Vector4(0.746f, 0.746f, 0.746f, 1.0f), "BackGroundTex");
	//黒色のテクスチャ―
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, blackColor, "BlackBackGroundTex");

	TextureManager::GetInstance()->LoadTexture("GameScene1.png");
	TextureManager::GetInstance()->LoadTexture("GameScene2.png");
	TextureManager::GetInstance()->LoadTexture("NowLoading.png");


	const Vector2 selectPosition = { 0.0f,160.0f };
	
	const Vector2 testPosition = { 0.0f,0.0f };

	const Vector2 backGroundPositionL = { 20.0f,180.0f };
	const Vector2 backGroundPositionR = { 660.0f,180.0f };

	//backGroundLeft_->Initialize("BackGroundTex",backGroundPositionL);
	//backGroundRight_->Initialize("BackGroundTex",backGroundPositionR);
	const Vector2 InitSpritePos = { 0.0f,0.0f };
	backGroundWhite_->Initialize("WhiteTex", InitSpritePos);
	selectSprite_->Initialize("CursorTex", selectPosition);
	gameSceneSprite1_->Initialize("GameScene1.png",backGroundPositionL);
	gameSceneSprite2_->Initialize("GameScene2.png",backGroundPositionR);
	//sceneTransition_->Initialize("BlackBackGroundTex", InitSpritePos);
	nowLoadingSprite_->Initialize("NowLoading.png", InitSpritePos);
	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	SoundManager::GetInstance()->Initialize();
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
	/*backGroundLeft_->matUpdate();
	backGroundRight_->matUpdate();*/
	backGroundWhite_->matUpdate();
	gameSceneSprite1_->matUpdate();
	gameSceneSprite2_->matUpdate();
	//sceneTransition_->matUpdate();
	nowLoadingSprite_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();

	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		touchSound_->PlaySoundWave(false);
		isChangeScene_ = true;
	}

	//カーソルのスプライトを移動させる
	const float kWindowHalfX = WindowsAPI::kWindow_width_ / 2;
	const float kSelectSpritePos = 160.0f;


	if(keys_->PushedKeyMoment(DIK_RIGHT))
	{
		if(selectSprite_->GetPosition().x == 0.0f)
		{
			selectSprite_->SetPosition(Vector2(kWindowHalfX, kSelectSpritePos));
		}
	}
	else if(keys_->PushedKeyMoment(DIK_LEFT))
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
			
			SoundManager::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");

			/*SoundManager::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");*/
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
	//backGroundLeft_->Draw("BackGroundTex");
	//backGroundRight_->Draw("BackGroundTex");
	gameSceneSprite1_->Draw("GameScene1.png");
	gameSceneSprite2_->Draw("GameScene2.png");

	if(isChangeScene_ == true)
	{
		//sceneTransition_->Draw("BlackBackGroundTex");
		nowLoadingSprite_->Draw("NowLoading.png");
	}
	
	//描画終了
	directXBasic_->AfterDraw();

}