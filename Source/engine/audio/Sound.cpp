#include "Sound.h"
#include "SoundManager.h"
#include <mutex>

#pragma comment(lib,"xaudio2.lib")

Sound::Sound() {}

Sound::~Sound() {}

void Sound::Initialize(const std::string& fileName)
{
	//soundData_ = SoundManager::GetInstance()->GetSoundData(fileName);
	soundData_ = SoundManager::GetInstance()->GetSoundData(fileName);
}

void Sound::PlaySoundWave(bool isLoop)
{
	HRESULT result;
	 
	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = SoundManager::GetInstance()->GetXAudio2()->CreateSourceVoice(&pSourceVoice, &soundData_.wfex);
	assert(SUCCEEDED(result));

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
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

//void Sound::LoadSoundWave(const std::string& fileName)
//{
//	//重複読み込みチェック
//	if (soundDatas_.find(fileName) != soundDatas_.end())
//	{
//		return;
//	}
//
//	//--ファイルオープン
//	//ディレクトリパスとファイル名を連結してフルパスを得る
//	const std::string directoryPath = "Resources/Sound/";
//	std::string fullPath = directoryPath + fileName;
//	//ファイル入力ストリームのインスタンス
//	std::ifstream file;
//	//.wavファイルをバイナリモードで開く
//	file.open(fullPath, std::ios_base::binary);
//	//ファイルオープン失敗を検出する
//	assert(file.is_open());
//
//	//--.wavデータ読み込み
//
//#pragma region RIFFチャンク読み込み
//	//RIFFヘッダーの読み込み
//	Sound::RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	//ファイルがRIFFかチェック
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
//	{
//		assert(0);
//	}
//	//タイプがWAVEかチェック
//	if (strncmp(riff.type, "WAVE", 4) != 0)
//	{
//		assert(0);
//	}
//#pragma endregion
//
//#pragma region FORMATチャンク読み込み
//	//FORMATチャンクの読み込み
//	Sound::FormatChunk format = {};
//	//チャンクヘッダーの確認
//	file.read((char*)&format, sizeof(Sound::ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//チャンク本体の読み込み
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char*)&format.fmt, format.chunk.size);
//#pragma endregion
//
//#pragma region DATAチャンク読み込み
//
//	Sound::ChunkHeader data;
//	file.read((char*)&data, sizeof(data));
//	//JUNKチャンクを検出した場合
//	if (strncmp(data.id, "JUNK", 4) == 0)
//	{
//		//読み取り位置をJUNKチャンクの終わりまで進める
//		file.seekg(data.size, std::ios_base::cur);
//		//再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	if (strncmp(data.id, "data", 4) != 0)
//	{
//		assert(0);
//	}
//
//	//DATAチャンクのデータ部(波形データ)の読み込み
//	std::vector<char> pBuffer(data.size);
//	file.read(pBuffer.data(), data.size);
//	
//#pragma endregion
//
//	//--ファイルを閉じる
//	file.close();
//
//	//--読み込んだ音声データをreturn
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = std::vector<BYTE>(pBuffer.begin(),pBuffer.end());
//	soundData.bufferSize = data.size;
//
//	//サウンドデータを連想配列(map)に格納
//	soundDatas_.insert(std::make_pair(fileName, soundData));
//}

//音声データ解放
//void Sound::UnloadSound(SoundData* soundData)
//{
//	//バッファのメモリを解放
//	soundData->pBuffer = { 0 };
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
////音声再生
//void Sound::PlaySoundWave(const std::string& fileName, bool isLoop)
//{
//	HRESULT result;
//
//	std::map<std::string, SoundData>::iterator it = soundDatas_.find(fileName);
//	//見読み込みの検出
//	assert(it != soundDatas_.end());
//	//サウンドデータの参照を取得
//	SoundData& soundData = it->second;
//
//	//波形フォーマットを元にSourceVoiceの生成
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	//ディレクトリパスとファイル名を連結してフルパスを得る
//	const std::string directoryPath = "Resources/Sound/";
//	std::string fullPath = directoryPath + fileName;
//
//	
//	/*std::set<Voice*>::iterator itr = voices_.begin();*/
//	
//	/*for(uint32_t i = 0; i < indexVoice_; i++)
//	{
//		voice.at(i) = std::make_unique<Voice>();
//		voice.at(i)->fileName = fullPath;
//		voice.at(i)->sourceVoice = pSourceVoice;
//		voices_.emplace(voice.at(i).get());
//	}*/
//
//	// 再生中データ
//	voice = std::make_unique<Voice>();
//	voice->fileName = fullPath;
//	voice->sourceVoice = pSourceVoice;
//	// 再生中データコンテナに登録	
//	voices_.insert(voice.get());
//
//	//indexVoice_++;
//	//std::set<Voice*>::iterator itr = voices_.begin();
//	
//	//for(uint32_t i = 0; i < indexVoice_; i++)
//	//{
//	//	itr++;
//	//	(itr) = std::move(voice.get());
//	//}
//
//	//再生する波形データの設定
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer.data();
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//	
//	//ループ処理
//	if(isLoop)
//	{
//		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
//	}
//
//	//波形データの再生
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//}
//
//void Sound::StopSound(const std::string& fileName)
//{
//	// 再生中リストから検索
//	auto it = std::find_if(
//		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
//	// 発見
//	if(it != voices_.end())
//	{
//		(*it)->sourceVoice->DestroyVoice();
//
//		voices_.erase(it);
//	}
//}
//
//void Sound::PauseSound(const std::string& fileName)
//{
//	// 再生中リストから検索
//	auto it = std::find_if(
//		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
//	// 発見
//	if(it != voices_.end())
//	{
//		// 再生停止
//		(*it)->sourceVoice->Stop();
//	}
//}
//
//void Sound::ResumeWave(const std::string& fileName)
//{
//	// 再生中リストから検索
//	auto it = std::find_if(
//		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
//	// 発見
//	if(it != voices_.end())
//	{
//		// 再生再開
//		(*it)->sourceVoice->Start();
//	}
//}
//
//void Sound::SetVolume(const std::string& fileName, float volume)
//{
//	// 再生中リストから検索
//	auto it = std::find_if(
//		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->fileName == fileName; });
//	// 発見
//	if(it != voices_.end())
//	{
//		(*it)->sourceVoice->SetVolume(volume);
//	}
//}



//void Sound::Finalize()
//{
//	//XAudio2解放
//	xAudio2_.Reset();
//	
//	//音声データ解放
//	//イテレーターを回す
//	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
//	for (; it != soundDatas_.end(); it++)
//	{
//		UnloadSound(&it->second);
//	}
//
//	soundDatas_.clear();
//}