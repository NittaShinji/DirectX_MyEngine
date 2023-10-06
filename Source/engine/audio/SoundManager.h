#pragma once
#include "Sound.h"
#include<map>

class SoundManager final
{
private:

	SoundManager();
	~SoundManager();

public:

	//コピーコンストラクタの無効
	SoundManager(const SoundManager& soundManager) = delete;
	//代入演算子の無効
	SoundManager& operator=(const SoundManager& soundManager) = delete;

public:

	// 再生データ
	struct Voice
	{
		std::string fileName;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

public:

	static SoundManager* GetInstance()
	{
		static SoundManager instance;
		return &instance;
	}

	void Initialize();

	/// <sumary>
	/// WAV音声読み込み
	/// </sumary>
	/// <param name="filename">WAVファイル名</param>
	void LoadSoundWave(const std::string& fileName);

	//音声データ解放
	void UnloadSound(Sound::SoundData* soundData);

	//終了処理
	void Finalize();

private:

	//XAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	//サウンドデータコンテナ
	std::map<std::string, Sound::SoundData> soundDatas_;

	////サウンド
	//std::unique_ptr<SoundData> sound_;
	//再生中のサウンドデータコンテナ
	std::map<char, Voice> voices_;

public:

	Sound::SoundData GetSoundData(std::string fileName);

};

