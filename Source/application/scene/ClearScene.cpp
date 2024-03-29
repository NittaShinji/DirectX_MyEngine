#include "ClearScene.h"
#include "SceneManager.h"
#include "Easing.h"
#include "MathUtillity.h"
#include "TextureManager.h"
#include "GameTimer.h"

using namespace MathUtillty;
using namespace NsEngine;

DirectXBasic* ClearScene::directXBasic_ = nullptr;
ImGuiManager* ClearScene::imGuiManager_ = nullptr;

void ClearScene::Initialize()
{
	//インスタンスの生成
	end_ = std::make_unique<Sprite>();
	check_ = std::make_unique<Sprite>();
	aButton_ = std::make_unique<Sprite>();
	clearSprite_ = std::make_unique<Sprite>();

	//各画像の初期化
	Vector2 titlePosition = { 0.0f,0.0f };;
	end_->Initialize("WhiteTex",titlePosition);

	const Vector2 kInitCheckPos = { 656,0.0f };
	checkPosition_.x = kInitCheckPos.x;
	check_->Initialize("check.png", kInitCheckPos);

	const Vector2 aButtonSize = { 128.0f,128.0f };
	const Vector2 aButtonPosition = { 576.0f,565.0f };
	aButton_->Initialize("aButton.png",aButtonPosition);
	aButton_->SetSize(Vector2(aButtonSize));
	
	const Vector2 kClearSpritePosition = { 320.0f,320.0f };
	clearSprite_->Initialize("gameClear.png", kClearSpritePosition);

	//シェーダー読み込み
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();

	//コントローラの初期化
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//サウンドの初期化
	checkSound_ = std::make_unique<Sound>();
	checkSound_->Initialize("clear.wav");
	checkSound_->PlaySoundWave(false);
	
	//アンカーポイントの設定
	const Vector2 checkAnchorPoint = { 0.5f,0.5f };
	check_->SetAnchorPoint(checkAnchorPoint);

	//ゲームタイマーを初期化
	GameTimer::GetInstance()->ResultInitialize(WindowsAPI::kWindow_height_ / 2);

	//変数
	move_ = { 0.0f,0.0f };
	isRotateSprite_ = false;
	isBackColor_ = false;
}

void ClearScene::Update()
{
	end_->matUpdate();
	aButton_->matUpdate();
	clearSprite_->matUpdate();

	//移動処理
	const float moveResultPosY = WindowsAPI::kWindow_height_ / 3 - 50;

	if(checkPosition_.y <= moveResultPosY)
	{
		const float kMoveSpeed = 0.6f;
		move_.y += kMoveSpeed;
		checkPosition_.y += move_.y;
	}

	//回転処理
	const float kOneRotationAngle = 360.0f;
	if(rotate_ <= ToRadian(kOneRotationAngle) && isRotateSprite_ == false)
	{
		float angle = ToRadian(kOneRotationAngle);
		rotate_ -= PlayEaseInCubic(0.0f, angle, rotateTimer_, kRotateTime_);
	}

	//チェック画像の回転処理
	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else if(rotateTimer_ < 0)
	{
		rotateTimer_ = kRotateTime_;
		isRotateSprite_ = true;
	}
	check_->SetPosition(checkPosition_);
	check_->SetRotation(rotate_);
	check_->matUpdate();

	//ゲームパッドが繋がっているかどうか
	if(gamePad_->IsConnected(Player1)) {}

	if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A) || keys_->PushedKeyMoment(DIK_RETURN))
	{
		if(isBackColor_ == false)
		{
			isPushedAbutton_ = true;
		}
	}

	//ボタンを押した際に色が変わるように
	if(isPushedAbutton_ == true || isBackColor_ == true)
	{
		const float changeColorSpeed = 0.02f;
		const float changeSceneColor = 0.7f;
		Vector4 aButtonChangeColor = aButton_->GetColor();

		//色を変更
		if(isPushedAbutton_ == true)
		{
			aButtonChangeColor.x -= changeColorSpeed;
			aButtonChangeColor.y -= changeColorSpeed;
			aButtonChangeColor.z -= changeColorSpeed;
		}
		else if(isBackColor_ == true)
		{
			aButtonChangeColor.x += changeColorSpeed;
			aButtonChangeColor.y += changeColorSpeed;
			aButtonChangeColor.z += changeColorSpeed;
		}
		aButton_->SetColor(aButtonChangeColor);

		//切り替え、終了処理
		if(isBackColor_ == false && aButtonChangeColor.x < changeSceneColor)
		{
			isBackColor_ = true;
			isPushedAbutton_ = false;
		}
		if(isBackColor_ == true && aButtonChangeColor.x > 1.0f)
		{
			SoundManager::GetInstance()->StopAllSound();
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}	
	}
	
	GameTimer::GetInstance()->ResultUpdate(true, WindowsAPI::kWindow_height_ / 2);
}

void ClearScene::Draw()
{
	//描画開始
	directXBasic_->BeforeDraw();

	//スプライトの描画
	SpriteCommon::GetInstance()->BeforeDraw();
	end_->Draw("WhiteTex");
	check_->Draw("check.png");
	aButton_->Draw("aButton.png");
	clearSprite_->Draw("gameClear.png");
	
	//描画終了
	directXBasic_->AfterDraw();
}

void ClearScene::StaticInitialize()
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;
}
