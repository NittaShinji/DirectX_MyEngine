#include "Sound.h"

#pragma comment(lib,"xaudio2.lib")

//Microsoft::WRL::ComPtr<IXAudio2> Sound::xAudio2_;

Sound::Sound(){}

Sound::~Sound(){}

void Sound::Initialize()
{
	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//マスターボイスの生成
	IXAudio2MasteringVoice* masterVoice_;
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(result));
}

void Sound::LoadSoundWave(const std::string& fileName)
{
	//重複読み込みチェック
	if (soundDates_.find(fileName) != soundDates_.end())
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
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = std::vector<BYTE>(pBuffer.begin(),pBuffer.end());
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列(map)に格納
	soundDates_.insert(std::make_pair(fileName, soundData));
}

//音声データ解放
void Sound::UnloadSound(SoundData* soundData)
{
	//バッファのメモリを解放
	//soundData->bufferSize = 0;
	//soundData->wfex = {};
}

//音声再生
void Sound::PlaySoundWave(const std::string& fileName)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDates_.find(fileName);
	//見読み込みの検出
	assert(it != soundDates_.end());
	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer.data();
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

}

void Sound::Finalize()
{
	//XAudio2解放
	xAudio2_.Reset();
	
	//音声データ解放
	//イテレーターを回す
	std::map<std::string, SoundData>::iterator it = soundDates_.begin();
	for (; it != soundDates_.end(); it++)
	{
		UnloadSound(&it->second);
	}

	soundDates_.clear();
}