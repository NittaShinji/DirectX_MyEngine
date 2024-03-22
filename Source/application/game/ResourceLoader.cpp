#include "ResourceLoader.h"
#include "Player.h"
#include "SoundManager.h"
using namespace NsEngine;

void ResourceLoader::Load()
{
	SoundManager::GetInstance()->LoadSoundWave("title.wav");
	SoundManager::GetInstance()->LoadSoundWave("touch.wav");
	SoundManager::GetInstance()->LoadSoundWave("gamescene.wav");
	SoundManager::GetInstance()->LoadSoundWave("jump.wav");
	SoundManager::GetInstance()->LoadSoundWave("doubleJump.wav");
	SoundManager::GetInstance()->LoadSoundWave("playerDead.wav");
	SoundManager::GetInstance()->LoadSoundWave("clear.wav");
}
