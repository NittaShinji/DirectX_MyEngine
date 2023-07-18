#include "MyFramework.h"
#include "TitleScene.h"
#include "GameScene.h"
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

	//ImGui初期化処理
	/*imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), directXBasic_.get());*/

	//サウンド処理
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスの生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	
	//スプライト
	SpriteCommon::GetInstance()->Initialize(directXBasic_.get());
	Sprite::StaticInitialize();

	//サウンド
	Sound::GetInstance()->Initialize();

	//基盤シーン静的初期化
	//scene_ = std::make_unique<TitleScene>();
	//scene_->StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	////タイトルシーン初期化処理
	//scene_->Initialize();

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
	//scene_->Update();
}

void MyFramework::Finalize()
{
	Sound::GetInstance()->Finalize();
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
