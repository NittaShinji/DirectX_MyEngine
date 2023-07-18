#include "MyFramework.h"
#include "xaudio2.h"
#include <wrl.h>

using namespace Microsoft::WRL;

void MyFramework::Initialize()
{
	//WindowsApi初期化処理
	winApi_ = std::make_unique<WindowsAPI>();
	winApi_->Initialize();

	//DirectX初期化処理
	directXBasic_ = std::make_unique<DirectXBasic>();
	directXBasic_->Initialize(winApi_.get());
	
	//Input初期化処理
	input_ = std::make_unique<Input>();
	input_->Initialize(winApi_.get());

	//サウンド処理
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスの生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void MyFramework::Update()
{
	//windowsメッセージ処理
	if(winApi_->ProcessMessage())
	{
		//終了メッセージが来たらゲームループを抜けるフラグをONにした
		endRequst_ = true;
	}

	input_->Update();
}

void MyFramework::Finalize()
{
	//ゲーム全体の終了処理
	winApi_->Finalize();
}

void MyFramework::Run()
{
	//ゲームの初期化
	Initialize();

	while(true)
	{
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if(IsEndRequst()){
			break;
		}
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}
