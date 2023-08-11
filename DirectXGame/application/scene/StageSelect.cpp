#include "StageSelect.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"

DirectXBasic* StageSelectScene::directXBasic_ = nullptr;
ImGuiManager* StageSelectScene::imGuiManager_ = nullptr;

void StageSelectScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

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
	BackGroundTest_ = std::make_unique<Sprite>();

	SpriteCommon::GetInstance()->LoadTexture("StageSelect.png");
	SpriteCommon::GetInstance()->LoadTexture("SelectBackGround.png");
	SpriteCommon::GetInstance()->TexMapping(128, 128,"RedTex");
	
	Vector2 selectPosition = { 0.0f,0.0f };
	const Vector2 selectSize = { 1280.0f,720.0f };
	selectSprite_->Initialize(selectPosition, selectSize);

	Vector2 testPosition = { 640.0f,0.0f };
	const Vector2 testSize = { 128.0f,128.0f };
	BackGroundTest_->Initialize(testPosition, testSize);

	Vector2 backGroundPositionL = { 20.0f,180.0f };
	Vector2 backGroundPositionR = { 660.0f,180.0f };
	const Vector2 backGroundSize = { 600.0f,360.0f };
	BackGroundLeft_->Initialize(backGroundPositionL, backGroundSize);
	BackGroundRight_->Initialize(backGroundPositionR, backGroundSize);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//サウンド
	Sound::GetInstance()->Initialize();
	//Sound::GetInstance()->LoadSoundWave("title.wav");
	Sound::GetInstance()->LoadSoundWave("touch.wav");

	//Sound::GetInstance()->PlaySoundWave("title.wav");
}

void StageSelectScene::Update()
{
	//画像の更新
	selectSprite_->matUpdate();
	BackGroundLeft_->matUpdate();
	BackGroundRight_->matUpdate();
	BackGroundTest_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}
	//押した瞬間の判定を取る
	gamePad_->PushedButtonMoment();

	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		Sound::GetInstance()->PlaySoundWave("touch.wav");
		isChangeScene_ = true;
	}

	if(keys_->PushedKeyMoment(DIK_RIGHT))
	{

	}
	else if(keys_->PushedKeyMoment(DIK_LEFT))
	{

	}

	if(isChangeScene_ == true)
	{
		changeSceneTimer_--;
		if(changeSceneTimer_ <= 0)
		{
			Sound::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}

void StageSelectScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	selectSprite_->Draw("StageSelect.png");
	//BackGroundLeft_->Draw("SelectBackGround.png");
	//BackGroundRight_->Draw("SelectBackGround.png");
	BackGroundTest_->Draw("RedTex");
}