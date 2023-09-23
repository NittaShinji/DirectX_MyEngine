#pragma once
#include "xaudio2.h"
#include <fstream>
#include <wrl.h>
#include <cassert>
#include <map>
#include <string>
#include <stdint.h>
#include <vector>

class Sound final
{

private:

	Sound();
	~Sound();

public:

	//コピーコンストラクタの無効
	Sound(const Sound& sound) = delete;
	//代入演算子の無効
	Sound& operator=(const Sound& sound) = delete;

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

public: //メンバ関数

	static Sound* GetInstance()
	{
		static Sound instance;
		return &instance;
	}

	void Initialize();
	
	/// <sumary>
	/// WAV音声読み込み
	/// </sumary>
	/// <param name="filename">WAVファイル名</param>
	//void LoadSoundWave(const char* fileName);
	void LoadSoundWave(const std::string& fileName);

	//音声データ解放
	void UnloadSound(SoundData* soundData);

	//終了処理
	void Finalize();

	//音声再生
	void PlaySoundWave(const std::string& fileName, bool isLoop);

private:

	//XAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	
	//サウンドデータ
	std::map<std::string, SoundData> soundDates_;
	
};

