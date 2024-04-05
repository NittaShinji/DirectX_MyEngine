#pragma once
#include "Sound.h"
#include <map>
#include <unordered_map>

/// <summary>
/// サウンドマネージャー
/// サウンドを管理する
/// </summary>
namespace NsEngine
{
	class SoundManager 
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

		//静的インスタンスを取得
		static SoundManager* GetInstance()
		{
			static SoundManager instance;
			return &instance;
		}

		//初期化
		void Initialize();

		//更新
		void Update();

		/// <sumary>
		/// WAV音声読み込み
		/// </sumary>
		/// <param name="filename">WAVファイル名</param>
		void LoadSoundWave(const std::string& fileName);

		void RegisterSound(std::string fileName);

		//再生データを登録
		void RegisterSEVoice(Sound::Voice* voice);
		void RegisterBGMVoice(Sound::Voice* voice);

		void StopSEVoice();
		void StopBGMVoice();

		void StopVoice(Sound::Voice* voice);

		//音声データを全てストップ
		void StopAllSound();

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

		//BGMサウンド
		std::vector<Sound*> soundsSE_;
		//SEサウンド
		std::vector<Sound*> soundsBGM_;

		//ソースボイス
		std::vector<Sound::Voice*> voicesSE_;
		std::vector<Sound::Voice*> voicesBGM_;

	public: //アクセッサ

		/// <summary>
		/// サウンドインスタンスを取得
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		std::unique_ptr<Sound> GetSound(std::string fileName);

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
}