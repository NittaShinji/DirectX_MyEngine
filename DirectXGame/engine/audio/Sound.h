#pragma once
#include "xaudio2.h"
#include <fstream>
#include <wrl.h>
#include <cassert>
#include <map>
#include <string>
#include <stdint.h>
#include <vector>
#include <set>

class Sound
{

public:

	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];			//チャンク毎のID
		int32_t size;		//チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	//"RIFF"
		char type[4];		//"WAVE"
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;	//"FMT"
		WAVEFORMATEX fmt;	//波形フォーマット
	};

	//音楽データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		std::vector<BYTE> pBuffer;
		//バッファのサイズ
		uint32_t bufferSize;
	};

	// 再生データ
	struct Voice
	{
		std::string fileName;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

public: //メンバ関数

	Sound();
	~Sound();

	static Sound* GetInstance()
	{
		static Sound instance;
		return &instance;
	}

	/// <sumary>
	/// WAV音声読み込み
	/// </sumary>
	/// <param name="filename">WAVファイル名</param>
	//void LoadSoundWave(const std::string& fileName);

	//音声データ解放
	//void UnloadSound(SoundData* soundData);

	//終了処理
	//void Finalize();

	void Initialize(const std::string& fileName);

	//音声再生
	void PlaySoundWave(const std::string& fileName, bool isLoop);

	//音声停止
	void StopSound(const std::string& fileName);

	//音声一時停止
	void PauseSound(const std::string& fileName);

	//一時停止からの再開
	void ResumeWave(const std::string& fileName);

	//音量調節
	void SetVolume(const std::string& fileName, float volume);

private:

	//サウンドデータ
	SoundData soundData_;
};

