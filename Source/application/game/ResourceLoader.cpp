#include "ResourceLoader.h"
#include "Player.h"
#include "SoundManager.h"
using namespace NsEngine;

void ResourceLoader::Load()
{
	SoundLoad();
	ImageLoad();
}

void ResourceLoader::SoundLoad()
{
	SoundManager::GetInstance()->LoadSoundWave("title.wav");
	SoundManager::GetInstance()->LoadSoundWave("touch.wav");
	SoundManager::GetInstance()->LoadSoundWave("gamescene.wav");
	SoundManager::GetInstance()->LoadSoundWave("jump.wav");
	SoundManager::GetInstance()->LoadSoundWave("doubleJump.wav");
	SoundManager::GetInstance()->LoadSoundWave("playerDead.wav");
	SoundManager::GetInstance()->LoadSoundWave("clear.wav");
}

void ResourceLoader::ImageLoad()
{
	//タイトルシーン
	TextureManager::GetInstance()->LoadTexture("titleFont.png");
	TextureManager::GetInstance()->LoadTexture("aButton.png");
	TextureManager::GetInstance()->LoadTexture("bButton.png");
	TextureManager::GetInstance()->LoadTexture("click.png");
	//ゲームシーン
	//読み込み処理
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
	TextureManager::GetInstance()->LoadTexture("nowLoading.png");

	//時計のテクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("stopWatch.png");
	//数字のテクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("numbers.png");

	//クリアシーン
	TextureManager::GetInstance()->LoadTexture("check.png");
	TextureManager::GetInstance()->LoadTexture("aButton.png");
	TextureManager::GetInstance()->LoadTexture("gameClear.png");
}
