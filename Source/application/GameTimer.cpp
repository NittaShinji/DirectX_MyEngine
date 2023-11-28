#include "GameTimer.h"
#include "TextureManager.h"

void GameTimer::Initialize()
{
	inGameTime_ = 0.0f;
	resultTime_ = 0.0f;
	highScoreTime_ = 0.0f;
	LoadNumTexture();
	TextureManager::GetInstance()->LoadTexture("clock.png");
	clock_ = std::make_unique<Sprite>();
	//clock_->Initialize();
}

void GameTimer::Update()
{

}

void GameTimer::Reset()
{

}

void GameTimer::LoadNumTexture()
{
	TextureManager::GetInstance()->LoadTexture("number/0.png");
	TextureManager::GetInstance()->LoadTexture("number/1.png");
	TextureManager::GetInstance()->LoadTexture("number/2.png");
	TextureManager::GetInstance()->LoadTexture("number/3.png");
	TextureManager::GetInstance()->LoadTexture("number/4.png");
	TextureManager::GetInstance()->LoadTexture("number/5.png");
	TextureManager::GetInstance()->LoadTexture("number/6.png");
	TextureManager::GetInstance()->LoadTexture("number/7.png");
	TextureManager::GetInstance()->LoadTexture("number/8.png");
	TextureManager::GetInstance()->LoadTexture("number/9.png");
}
