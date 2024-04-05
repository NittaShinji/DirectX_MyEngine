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
	//再生が終了した効果音を削除
	if (voicesSE_.size() != 0)
	{
		for (auto it = voicesSE_.begin(); it != voicesSE_.end(); )
		{
			Sound::Voice* voice = *it;
			XAUDIO2_VOICE_STATE state;
			voice->sourceVoice->GetState(&state);

			if (state.BuffersQueued == 0)
			{
				// 再生が終了したら停止してから解放
				voice->sourceVoice->Stop();
				
				// ベクターから削除
				it = voicesSE_.erase(it);
				// イテレーターを進めずに次の要素に対してチェックを行う
			}
			else
			{
				// 再生が終了していない場合は次の要素に進む
				++it;
			}
		}
	}
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

	//インスタンスを生成する
	RegisterSound(fileName);
}

void NsEngine::SoundManager::StopSEVoice()
{
	for (auto voice : voicesSE_)
	{
		StopVoice(voice);
	}
	voicesSE_.clear();
}
void NsEngine::SoundManager::StopBGMVoice()
{
	for (auto voice : voicesBGM_)
	{
		StopVoice(voice);
	}
	voicesBGM_.clear();
}

void SoundManager::RegisterSound(std::string fileName)
{
	std::unique_ptr<Sound> sound = nullptr;
	sound = std::make_unique<Sound>();
	sound->Initialize(fileName);

	std::string strSE = "SE";
	size_t foundSE = fileName.find(strSE);
	if (foundSE != std::string::npos) {
		soundsSE_.push_back(std::move(sound.get()));
		return;
	}

	std::string strBGM = "BGM";
	size_t foundBGM = fileName.find(strBGM);
	if (foundBGM != std::string::npos) {
		soundsBGM_.push_back(std::move(sound.get()));
		return;
	}
}

void NsEngine::SoundManager::RegisterSEVoice(Sound::Voice* voice)
{
	voicesSE_.push_back(voice);
}

void NsEngine::SoundManager::RegisterBGMVoice(Sound::Voice* voice)
{
	voicesBGM_.push_back(voice);
}

void SoundManager::StopAllSound()
{
	//全てのサウンドを停止
  	for (auto voice : voicesSE_)
	{
		StopVoice(voice);
	}
	for (auto voice : voicesBGM_)
	{
		StopVoice(voice);
	}

	voicesSE_.clear();
	voicesBGM_.clear();
}

void SoundManager::StopVoice(Sound::Voice* voice) {

	if (voice && voice->sourceVoice) {
		voice->sourceVoice->Stop();
	}
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


