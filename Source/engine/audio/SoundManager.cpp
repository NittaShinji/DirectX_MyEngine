#include "SoundManager.h"
using namespace NsEngine;

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

void SoundManager::Initialize()
{
	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//マスターボイスの生成
	IXAudio2MasteringVoice* masterVoice_;
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result));
}

void NsEngine::SoundManager::Update()
{
	//uint32_t length = static_cast<uint32_t>(voices_.size());
	//uint32_t beforeLength = length;
	//for (int i = 0; i < voices_.size();)
	//{
	//	if (voices_[i]->isLoop == false)
	//	{
	//		XAUDIO2_VOICE_STATE state;
	//		voices_[i]->sourceVoice->GetState(&state);
	//		if (state.BuffersQueued == 0)
	//		{
	//			voices_[i]->sourceVoice->DestroyVoice();
	//			voices_[i]->sourceVoice = nullptr;
	//			//リストから音声を削除
 //				voices_.erase(voices_.begin() + i);
	//		}
	//		else
	//		{
	//			i++;
	//		}
	//	}
	//	else
	//	{
	//		i++;
	//	}

	//}

	//for (int i = 0; i < voices_.size();)
	//{
	//	//ループじゃなければ
	//	if (voices_[i]->isLoop == false)
	//	{
	//		XAUDIO2_VOICE_STATE state;
	//		if (voices_[i]->sourceVoice != nullptr)
	//		{
	//			voices_[i]->sourceVoice->GetState(&state);
	//			//再生が終了しているか
	//			if (state.BuffersQueued == 0)
	//			{
	//				// ソースボイスがまだ有効な場合のみ、破棄する
	//				if (voices_[i]->sourceVoice)
	//				{
	//					voices_[i]->sourceVoice->DestroyVoice();
	//					voices_[i]->sourceVoice = nullptr; // ヌルポインタに設定することで、重複して解放されないようにする
	//				}

	//				// リストから音声を削除
	//				voices_.erase(voices_.begin() + i);
	//			}
	//			else
	//			{
	//				// リストから音声を削除
	//				voices_.erase(voices_.begin() + i);
	//			}
	//		}
	//		else
	//		{
	//			i++;
	//		}
	//	}
	//	else
	//	{
	//		i++;
	//	}
	//}

	//for (int i = 0; i < voices_.size();)
	//{
	//	//ループじゃなければ
	//	if (voices_[i]->isLoop == false)
	//	{
	//		if (voices_[i]->sourceVoice != nullptr)
	//		{
	//			voices_.erase(voices_.begin() + i);
	//		}
	//		else
	//		{
	//			i++;
	//		}
	//	}
	//	else
	//	{
	//		i++;
	//	}
	//}

	//これ実行できる
	//for (int i = 0; i < voices_.size();i++)
	//{
	//	//ループじゃなければ
	//	/*if (voices_[i]->isLoop == false)
	//	{
	//		voices_.erase(voices_.begin() + i);
	//	}
	//	else
	//	{
	//		i++;
	//	}*/
	//}
}

void SoundManager::LoadSoundWave(const std::string& fileName)
{
	//重複読み込みチェック
	if (soundDatas_.find(fileName) != soundDatas_.end())
	{
		return;
	}

	//--ファイルオープン
	//ディレクトリパスとファイル名を連結してフルパスを得る
	const std::string directoryPath = "Resources/Sound/";
	std::string fullPath = directoryPath + fileName;
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(fullPath, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//--.wavデータ読み込み

#pragma region RIFFチャンク読み込み
	//RIFFヘッダーの読み込み
	Sound::RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}
#pragma endregion

#pragma region FORMATチャンク読み込み
	//FORMATチャンクの読み込み
	Sound::FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(Sound::ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
#pragma endregion

#pragma region DATAチャンク読み込み

	Sound::ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//DATAチャンクのデータ部(波形データ)の読み込み
	std::vector<char> pBuffer(data.size);
	file.read(pBuffer.data(), data.size);

#pragma endregion

	//--ファイルを閉じる
	file.close();

	//--読み込んだ音声データをreturn
	Sound::SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = std::vector<BYTE>(pBuffer.begin(), pBuffer.end());
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列(map)に格納
	soundDatas_.insert(std::make_pair(fileName, soundData));
}

void SoundManager::RegisterVoice(Sound::Voice* voice)
{
	voices_.push_back(voice);
}

void SoundManager::StopAllSound()
{
	for (int i = 0; i < voices_.size(); i++)
	{
		voices_[i]->sourceVoice->Stop();
	}
	voices_.resize(0);
}

void SoundManager::UnloadParticularSound(const std::string& fileName)
{
	std::map<std::string, Sound::SoundData>::iterator it = soundDatas_.find(fileName);
	//見読み込みの検出
	assert(it != soundDatas_.end());
	//サウンドデータを取得
	Sound::SoundData* soundData = &it->second;
	//バッファのメモリを解放
	soundData->pBuffer = { 0 };
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

std::unique_ptr<Sound> SoundManager::GetSound(std::string fileName)
{
	std::unique_ptr<Sound> sound = std::make_unique<Sound>();
	sound->Initialize(fileName);

	return sound;
}

//キーに対応したサウンドデータを返す
Sound::SoundData& SoundManager::GetSoundData(std::string fileName)
{
	std::map<std::string, Sound::SoundData>::iterator it = soundDatas_.find(fileName);
	//見読み込みの検出
	assert(it != soundDatas_.end());
	//サウンドデータを取得
	Sound::SoundData& soundData = it->second;

	return soundData;
}

//音声データ解放
void SoundManager::UnloadSound(Sound::SoundData* soundData)
{
	//バッファのメモリを解放
	soundData->pBuffer = { 0 };
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void SoundManager::Finalize()
{
	//XAudio2解放
	xAudio2_.Reset();

	//音声データ解放
	//イテレーターを回す
	std::map<std::string, Sound::SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); it++)
	{
		UnloadSound(&it->second);
	}

	soundDatas_.clear();
}


