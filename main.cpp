
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
	//WindowsApi初期化処理
	std::unique_ptr<WindowsAPI> winApi = std::make_unique<WindowsAPI>();
	winApi->Initialize();

	//DirectX初期化処理
	std::unique_ptr<DirectXBasic> directXBasic = std::make_unique<DirectXBasic>();
	directXBasic->Initialize(winApi.get());

	//ImGui初期化処理
	std::unique_ptr<ImGuiManager> imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApi.get(), directXBasic.get());

	//------サウンド処理

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスの生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//------入力処理
	std::unique_ptr<Input> input = std::make_unique<Input>();
	input->Initialize(winApi.get());

	//ゲームシーン
	std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	gameScene->Initialize(directXBasic.get(), imGuiManager.get());

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
	winApi->Finalize();
	
#pragma endregion WindowsAPI後始末

	return 0;
}