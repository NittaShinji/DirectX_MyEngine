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
	SoundManager::GetInstance()->LoadSoundWave("titleBGM.wav");
	SoundManager::GetInstance()->LoadSoundWave("gamesceneBGM.wav");
	SoundManager::GetInstance()->LoadSoundWave("clearBGM.wav");
	SoundManager::GetInstance()->LoadSoundWave("touchSE.wav");
	SoundManager::GetInstance()->LoadSoundWave("jumpSE.wav");
	SoundManager::GetInstance()->LoadSoundWave("playerDeadSE.wav");
}

void ResourceLoader::ImageLoad()
{
	//<タイトルシーン>
	//白色のテクスチャ―を生成
	const int32_t backGroundWidth = 1280;
	const int32_t backGroundHeight = 720;
	const Vector4 whiteColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//画像読み込み
	TextureManager::GetInstance()->TexMapping(backGroundWidth, backGroundHeight, whiteColor, "WhiteTex");
	TextureManager::GetInstance()->LoadTexture("titleFont.png");
	TextureManager::GetInstance()->LoadTexture("aButton.png");
	TextureManager::GetInstance()->LoadTexture("bButton.png");
	TextureManager::GetInstance()->LoadTexture("click.png");
	TextureManager::GetInstance()->LoadTexture("xboxLt.png");
	//<ゲームシーン>
	//黒色のテクスチャ―を生成
	const int32_t kHalfWindowHeight = WindowsAPI::kWindow_height_ / 2;
	const Vector4 blackColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	//画像読み込み
	TextureManager::GetInstance()->TexMapping(WindowsAPI::kWindow_width_, kHalfWindowHeight, blackColor, "BlackBackGroundHalfTex");
	TextureManager::GetInstance()->LoadTexture("jump.png");
	TextureManager::GetInstance()->LoadTexture("arrow.png");
	TextureManager::GetInstance()->LoadTexture("cloud.png");
	TextureManager::GetInstance()->LoadTexture("jumpEffect6.png");
	TextureManager::GetInstance()->LoadTexture("backGround.png");
	TextureManager::GetInstance()->LoadTexture("nowLoading.png");
	TextureManager::GetInstance()->LoadTexture("next.png");
	TextureManager::GetInstance()->LoadTexture("stage.png");
	TextureManager::GetInstance()->LoadTexture("circle.png");
	//時計のテクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("stopWatch.png");
	//王冠のテクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("crown.png");
	//数字のテクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("numbers.png");
	TextureManager::GetInstance()->LoadTexture("checkBox.png");
	//小数点用のテクスチャ
	const int blackDotSize = 6;
	const int grayDotSize = 8;
	const Vector4 grayColor = { 0.45f, 0.45f, 0.45f, 1.0f };
	TextureManager::GetInstance()->TexMapping(blackDotSize, blackDotSize, blackColor, "BlackDot");
	TextureManager::GetInstance()->TexMapping(grayDotSize, grayDotSize, grayColor, "GrayDot");

	//<クリアシーン>
	TextureManager::GetInstance()->LoadTexture("check.png");
	TextureManager::GetInstance()->LoadTexture("gameClear.png");
}
