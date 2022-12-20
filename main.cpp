
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
#include <memory>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();
	WindowsAPI* winApi = nullptr;
	winApi = new WindowsAPI();
	winApi->Initialize();

	std::unique_ptr<Input> input = std::make_unique<Input>();
	//std::unique_ptr<DirectXBasic> directBasic_ = std::make_unique<DirectXBasic>();
	DirectXBasic* directXBasic = nullptr;
	directXBasic = new DirectXBasic();

	//DirectX初期化処理
	directXBasic->Initialize(winApi);

	Model::StaticInitialize(directXBasic);

	//シングルトンインスタンスを作成
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);

	//3Dオブジェクト生成
	Object3d* object3d = nullptr;
	object3d = new Object3d("Resources/triangle/triangle.obj",directXBasic,0, "tomas.png");
	//object3d->LoadTexture(0, "tomas.png");

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

		object3d->Update();

		directXBasic->BeforeDraw();
		object3d->BeforeDraw();

		object3d->Draw();

		object3d->AfterDraw();
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI後始末
	//ゲーム全体の終了処理

	delete winApi;
	delete directXBasic;
	delete object3d;
#pragma endregion WindowsAPI後始末

	winApi = nullptr;
	directXBasic = nullptr;
	//input = nullptr;
	return 0;
}