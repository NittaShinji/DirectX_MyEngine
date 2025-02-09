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
	Reset();
}

void Sound::Reset()
{
	isSounded_ = false;
	voice_.isLoop = false;
}

void Sound::PlaySoundWave(bool isLoop)
{
	HRESULT result;

	////再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_.pBuffer.data();
	buf.AudioBytes = soundData_.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//ループ処理
	if (isLoop)
	{
		voice_.sourceVoice = nullptr;
		result = SoundManager::GetInstance()->GetXAudio2()->CreateSourceVoice(&voice_.sourceVoice, &soundData_.wfex);
		assert(SUCCEEDED(result));

		voice_.isLoop = true;
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		//波形データの再生
		result = voice_.sourceVoice->SubmitSourceBuffer(&buf);
		result = voice_.sourceVoice->Start();

		//サウンドマネージャーにソースボイスを登録
		SoundManager::GetInstance()->RegisterBGMVoice(&voice_);
	}
	else
	{
		//波形フォーマットを元にSourceVoiceの生成
		voice_.sourceVoice = nullptr;
		result = SoundManager::GetInstance()->GetXAudio2()->CreateSourceVoice(&voice_.sourceVoice, &soundData_.wfex);
		assert(SUCCEEDED(result));

		// 再生が完了したかどうかを記録する
		isSounded_ = true;

		//波形データの再生
		result = voice_.sourceVoice->SubmitSourceBuffer(&buf);
		result = voice_.sourceVoice->Start();

		//サウンドマネージャーにソースボイスを登録
		SoundManager::GetInstance()->RegisterSEVoice(&voice_);
	}

	//鳴ったことを記録する
	isSounded_ = true;
}

void Sound::StopSound()
{
	if (voice_.sourceVoice)
	{
		voice_.sourceVoice->Stop();
	}
}

void Sound::Delete()
{
	if (voice_.sourceVoice)
	{
		voice_.sourceVoice->Stop();
		voice_.sourceVoice->DestroyVoice();
		voice_.sourceVoice = nullptr;
	}
}