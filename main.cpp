
//#include <d3d12.h>
//#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
using namespace DirectX;
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include "Input.h"
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "GameScene.h"
#include "xaudio2.h"
#include <memory>
#include <fstream>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"xaudio2.lib")

////チャンクヘッダ
//struct ChunkHeader
//{
//	char id[4];			//チャンク毎のID
//	int32_t size;		//チャンクサイズ
//};
//
////RIFFヘッダチャンク
//struct RiffHeader
//{
//	ChunkHeader chunk;	//"RIFF"
//	char type[4];		//"WAVE"
//};
//
////FMTチャンク
//struct FormatChunk
//{
//	ChunkHeader chunk;	//"FMT"
//	WAVEFORMATEX fmt;	//波形フォーマット
//};
//
////音楽データ
//struct SoundData
//{
//	//波形フォーマット
//	WAVEFORMATEX wfex;
//	//バッファの先頭アドレス
//	BYTE* pBuffer;
//	//バッファのサイズ
//	unsigned int bufferSize;
//};
//
//SoundData SoundLoadWave(const char* fileName)
//{
//	HRESULT result;
//
//	//--ファイルオープン
//
//	//ファイル入力ストリームのインスタンス
//	std::ifstream file;
//	//.wavファイルをバイナリモードで開く
//	file.open(fileName, std::ios_base::binary);
//	//ファイルオープン失敗を検出する
//	assert(file.is_open());
//
//	//--.wavデータ読み込み
//	 
//#pragma region RIFFチャンク読み込み
//	//RIFFヘッダーの読み込み
//	RiffHeader riff;
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
//	FormatChunk format = {};
//	//チャンクヘッダーの確認
//	file.read((char*)&format, sizeof(ChunkHeader));
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
//	ChunkHeader data;
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
//	char* pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//
//#pragma endregion
//
//	//--ファイルを閉じる
//	file.close();
//
//	//--読み込んだ音声データをreturn
//	
//	//returnする為の音声データ
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE*> (pBuffer);
//	soundData.bufferSize = data.size;
//
//	return soundData;
//}
//
////音声データ解放
//void SoundUnload(SoundData* soundData)
//{
//	//バッファのメモリを解放
//	delete[] soundData->pBuffer;
//
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
////音声再生
//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData)
//{
//	HRESULT result;
//
//	//波形フォーマットを元にSourceVoiceの生成
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	//再生する波形データの設定
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	//波形データの再生
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//
//}

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();
	
	WindowsAPI* winApi = nullptr;
	winApi = new WindowsAPI();
	winApi->Initialize();

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	std::unique_ptr<Input> input = std::make_unique<Input>();
	//std::unique_ptr<DirectXBasic> directBasic_ = std::make_unique<DirectXBasic>();
	DirectXBasic* directXBasic = nullptr;
	directXBasic = new DirectXBasic();

	//DirectX初期化処理
	directXBasic->Initialize(winApi);

	//------サウンド

	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスの生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//音声読み込み
	//SoundData testSound = SoundLoadWave("Resources/Alarm01.wav");

	//シングルトンインスタンスを作成
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);
	
	GameScene* gameScene = nullptr;
	gameScene = new GameScene;
	gameScene->Initialize(directXBasic);

	//SoundPlayWave(xAudio2.Get(), testSound);

	// ゲームループ
	while (true) {
		//windowsメッセージ処理
		if (winApi->ProcessMessage())
		{
			//終了メッセージが来たらゲームループを抜ける
			break;
		}

		// DirectX毎フレーム処理 ここから
		input->Update();

		gameScene->Update();

		//描画
		directXBasic->BeforeDraw();
		
		gameScene->Draw();
		
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI後始末
	//ゲーム全体の終了処理

	delete winApi;
	delete directXBasic;
	delete gameScene;
	
#pragma endregion WindowsAPI後始末

	winApi = nullptr;
	directXBasic = nullptr;
	
	return 0;
}