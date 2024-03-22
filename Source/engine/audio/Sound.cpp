#include "Sound.h"
#include "SoundManager.h"
#include <mutex>
#pragma comment(lib,"xaudio2.lib")

using namespace NsEngine;

Sound::Sound() {}

Sound::~Sound() {}

void Sound::Initialize(const std::string& fileName)
{
	soundData_ = SoundManager::GetInstance()->GetSoundData(fileName);

	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	voice_.sourceVoice = nullptr;
	result = SoundManager::GetInstance()->GetXAudio2()->CreateSourceVoice(&voice_.sourceVoice, &soundData_.wfex);
	assert(SUCCEEDED(result));

	SoundManager::GetInstance()->RegisterVoice(&voice_);
	isSounded_ = false;
}

void Sound::PlaySoundWave(bool isLoop)
{
	HRESULT result;

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_.pBuffer.data();
	buf.AudioBytes = soundData_.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//ループ処理
	if(isLoop)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//波形データの再生
	result = voice_.sourceVoice->SubmitSourceBuffer(&buf);
	result = voice_.sourceVoice->Start();
	
	//鳴ったことを記録する
	isSounded_ = true;
}

void Sound::StopSound()
{
	voice_.sourceVoice->Stop();
}

void Sound::Delete()
{
	voice_.sourceVoice->DestroyVoice();
	voice_.sourceVoice = nullptr;
}
