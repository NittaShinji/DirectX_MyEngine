
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
#include "ImGuiManager.h"
#include <memory>
#include <fstream>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"xaudio2.lib")

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

	//ImGui初期化処理
	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApi,directXBasic);


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

		imGuiManager->Begin();

		input->Update();

		gameScene->Update();

		imGuiManager->End();

		//描画
		directXBasic->BeforeDraw();
		
		gameScene->Draw();
		imGuiManager->Draw();
		
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI後始末
	//ゲーム全体の終了処理
	imGuiManager->Finalize();
	delete imGuiManager;
	delete winApi;
	delete directXBasic;
	delete gameScene;
	
#pragma endregion WindowsAPI後始末

	winApi = nullptr;
	directXBasic = nullptr;
	
	return 0;
}