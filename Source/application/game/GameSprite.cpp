#include "GameSprite.h"
#include "Easing.h"

void GameSprite::Load()
{
	//スプライト
	const int32_t kHalfWindowHeight = WindowsAPI::kWindow_height_ / 2;
	const Vector4 blackColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	//黒色のテクスチャ―を生成
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, kHalfWindowHeight, blackColor, "BlackBackGroundHalfTex");
	//画像読み込み
	TextureManager::GetInstance()->LoadTexture("jump.png");
	TextureManager::GetInstance()->LoadTexture("arrow.png");
	TextureManager::GetInstance()->LoadTexture("cloud.png");
	TextureManager::GetInstance()->LoadTexture("effect2.png");
	TextureManager::GetInstance()->LoadTexture("jumpEffect6.png");
	TextureManager::GetInstance()->LoadTexture("backGround.png");
}

void GameSprite::Initialize()
{
	isStartSceneAnimation_ = false;

	const int32_t kHalfWindowHeight = WindowsAPI::kWindow_height_ / 2;

	Load();

	//aボタン画像
	aButtonSprite_ = std::make_unique<Sprite>();
	const Vector2 aButtonPosition = { 1152,648 };
	aButtonSprite_->Initialize("A.png", aButtonPosition);
	aButtonSprite_->SetSize(Vector2(kUiSize_));
	//bボタン画像
	bButtonSprite_ = std::make_unique<Sprite>();
	const Vector2 bButtonPosition = { 1216,648 };
	bButtonSprite_->Initialize("B.png", bButtonPosition);
	bButtonSprite_->SetSize(Vector2(kUiSize_));
	//ジャンプ画像
	jumpSprite_ = std::make_unique<Sprite>();
	const Vector2 jumpSpritePosition = { 1152,584 };
	jumpSprite_->Initialize("jump.png", jumpSpritePosition);
	jumpSprite_->SetSize(Vector2(kUiSize_));
	//矢印画像
	arrowSprite_ = std::make_unique<Sprite>();
	const Vector2 arrowPosition = { 1216,584 };
	arrowSprite_->Initialize("arrow.png", arrowPosition);
	arrowSprite_->SetSize(Vector2(kUiSize_));
	//背景画像
	backGroundSprite_ = std::make_unique<Sprite>();
	const Vector2 kDefaultSpritePos = { 0.0f,0.0f };
	backGroundSprite_->Initialize("backGround.png", kDefaultSpritePos);
	//トランジション用画像
	nowLoadingSprite_ = std::make_unique<Sprite>();
	nowLoadingSprite_->Initialize("NowLoading.png", kDefaultSpritePos);

	sceneTransitionUp_ = std::make_unique<Sprite>();
	sceneTransitionDown_ = std::make_unique<Sprite>();
	sceneTransitionUp_->Initialize("BlackBackGroundHalfTex", kDefaultSpritePos);
	sceneTransitionDown_->Initialize("BlackBackGroundHalfTex", Vector2(kDefaultSpritePos.x, kHalfWindowHeight));
}

void GameSprite::Update()
{
	//スプライト
	aButtonSprite_->matUpdate();
	bButtonSprite_->matUpdate();
	jumpSprite_->matUpdate();
	arrowSprite_->matUpdate();
	backGroundSprite_->matUpdate();
	sceneTransitionUp_->matUpdate();
	sceneTransitionDown_->matUpdate();
	nowLoadingSprite_->matUpdate();
}

void GameSprite::BackGroundDraw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	backGroundSprite_->Draw("backGround.png");
}

void GameSprite::UIDraw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");
	jumpSprite_->Draw("jump.png");
	arrowSprite_->Draw("arrow.png");
}

void GameSprite::TransitionDraw()
{
	sceneTransitionUp_->Draw("BlackBackGroundHalfTex");
	sceneTransitionDown_->Draw("BlackBackGroundHalfTex");
	if(isStartSceneAnimation_ == false)
	{
		nowLoadingSprite_->Draw("NowLoading.png");
	}
}

void GameSprite::SceneAnimation()
{
	isStartSceneAnimation_ = true;

	if(sceneAnimeTimer_ < kSceneAnimeTime_)
	{
		sceneAnimeTimer_++;
	}
	else
	{
		sceneAnimeTimer_ = 0;
		isFinishAnimetion_ = true;
	}

	//変化量
	float x = sceneAnimeTimer_ / kSceneAnimeTime_;
	sceneAnimationVec_.y += EaseOutQuint(x);

	//画像を動かす処理
	sceneTransitionUp_->MovePos(-sceneAnimationVec_);
	sceneTransitionDown_->MovePos(sceneAnimationVec_);
}

void GameSprite::ResetSceneAnimation()
{
	if(isFinishAnimetion_ == true)
	{
		sceneTransitionUp_->SetPosition(Vector2(0.0f, 0.0f));
		const float kHalfWindowHeight = WindowsAPI::kWindow_height_ / 2;
		sceneTransitionDown_->SetPosition(Vector2(0.0f, kHalfWindowHeight));
		sceneAnimeTimer_ = 0;
		sceneAnimationVec_ = Vector2(0.0f, 0.0f);
		isFinishAnimetion_ = false;
	}
}
