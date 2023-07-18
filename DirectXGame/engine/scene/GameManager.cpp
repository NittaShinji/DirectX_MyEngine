#include "GameManager.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

GameManager::GameManager()
{
	//ゲームの初期化
	Initialize();
	//ゲームループ
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
	//ゲームの終了処理
	Finalize();
}

void GameManager::Initialize()
{
	//WindowsApi初期化処理
	winApi_ = std::make_unique<WindowsAPI>();
	winApi_->Initialize();

	//DirectX初期化処理
	directXBasic_ = std::make_unique<DirectXBasic>();
	directXBasic_->Initialize(winApi_.get());

	//ImGui初期化処理
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), directXBasic_.get());

	//サウンド処理
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2エンジンのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスの生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//Input初期化処理
	input_ = std::make_unique<Input>();
	input_->Initialize(winApi_.get());

	//GameScene初期化処理
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(directXBasic_.get(), imGuiManager_.get());

	//ポストエフェクト初期化処理
	postEffect_ = std::make_unique<PostEffect>();
	//ポストエフェクト用テクスチャの読み込み
	SpriteCommon::LoadTexture("test.png");
	postEffect_->Initialize(directXBasic_.get());
}

void GameManager::Update()
{
	//windowsメッセージ処理
	if(winApi_->ProcessMessage())
	{
		//終了メッセージが来たらゲームループを抜けるフラグをONにした
		endRequst_ = true;
	}

	// DirectX毎フレーム処理 ここから
	imGuiManager_->Begin();

	input_->Update();

	postEffect_->SetAnchorPoint(XMFLOAT2(0.0f, 0.0f));
	gameScene_->Update();

	imGuiManager_->End();
}

void GameManager::Draw()
{
	//レンダーテクスチャの描画
	postEffect_->PreDrawScene();
	gameScene_->Draw();
	postEffect_->PostDrawScene();

	//描画開始
	directXBasic_->BeforeDraw();
	//ポストエフェクトの描画
	postEffect_->Draw("test.png");
	imGuiManager_->Draw();

	//描画終了
	directXBasic_->AfterDraw();
}

void GameManager::Finalize()
{
	//ゲーム全体の終了処理
	imGuiManager_->Finalize();
	winApi_->Finalize();
}