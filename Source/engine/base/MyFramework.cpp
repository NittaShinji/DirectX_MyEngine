#include "MyFramework.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "xaudio2.h"
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace NsEngine;


void MyFramework::Initialize()
{
	//WindowsApi初期化処理
	winApi_ = WindowsAPI::GetInstance();
	winApi_->Initialize();

	//DirectX初期化処理
	directXBasic_ = std::make_unique<DirectXBasic>();
	directXBasic_->Initialize(winApi_);

	//Input初期化処理
	input_ = std::make_unique<Input>();
	input_->Initialize(winApi_);

	//ImGui初期化処理
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_, directXBasic_.get());

	//テクスチャマネージャ初期化
	TextureManager::GetInstance()->StaticInitialize(directXBasic_->GetDevice().Get());
	TextureManager::GetInstance()->Initialize();

	//スプライト
	SpriteCommon::GetInstance()->Initialize(directXBasic_.get());
	Sprite::StaticInitialize();

	//シーン静的初期化
	BaseScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
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
		if(IsEndRequst())
		{
			break;
		}
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}
