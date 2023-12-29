#pragma once
#include "Sound.h"
#include<map>

class SoundManager final
{
private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//コンストラクタ
	SoundManager();
	//デストラクタ
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

	//静的インスタンスを取得
	static SoundManager* GetInstance()
	{
		static SoundManager instance;
		return &instance;
	}

	//初期化
	void Initialize();

	/// <sumary>
	/// WAV音声読み込み
	/// </sumary>
	/// <param name="filename">WAVファイル名</param>
	void LoadSoundWave(const std::string& fileName);

	//特定の音声データを解放
	void UnloadParticularSound(const std::string& fileName);

	//音声データ解放
	void UnloadSound(Sound::SoundData* soundData);

	//終了処理
	void Finalize();

private:

	//XAudio2のインスタンス
	ComPtr<IXAudio2> xAudio2_;
	//サウンドデータコンテナ
	std::map<std::string, Sound::SoundData> soundDatas_;

	//サウンド
	//再生中のサウンドデータコンテナ
	std::map<char, Voice> voices_;

public: //アクセッサ

	/// <summary>
	/// サウンドデータを取得
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>サウンドデータ</returns>
	Sound::SoundData& GetSoundData(std::string fileName);

	/// <summary>
	/// サウンドデータマップを取得
	/// </summary>
	/// <returns>サウンドデータマップ</returns>
	std::map<std::string, Sound::SoundData> GetSoundDataMap() { return soundDatas_; }

	/// <summary>
	/// //XAudio2のインスタンスを取得
	/// </summary>
	/// <returns></returns>
	ComPtr<IXAudio2> GetXAudio2() { return xAudio2_.Get(); };
};