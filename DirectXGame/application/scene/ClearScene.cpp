#include "ClearScene.h"
#include "SceneManager.h"
#include "Sound.h"
#include "Easing.h"
#include "MathUtillity.h"
#include "TextureManager.h"

using namespace MathUtillty;

DirectXBasic* ClearScene::directXBasic_ = nullptr;
ImGuiManager* ClearScene::imGuiManager_ = nullptr;

void ClearScene::Initialize()
{
	end_ = std::make_unique<Sprite>();
	check_ = std::make_unique<Sprite>();
	aButton_ = std::make_unique<Sprite>();

	//SpriteCommon::GetInstance()->LoadTexture("end.png");
	TextureManager::GetInstance()->LoadTexture("check.png");
	TextureManager::GetInstance()->LoadTexture("A.png");

	Vector2 titlePosition = { 0.0f,0.0f };
	Vector2 titleSize = { 1280.0f,720.0f };
	end_->Initialize(titlePosition, titleSize);

	Vector2 checkSize = { 157.0f,112.0f };
	//checkPosition_.x = (WindowsAPI::kWindow_width_ / 2) - (checkSize.x / 2);
	checkPosition_.x = (WindowsAPI::kWindow_width_ / 2);

	//checkPosition_.y = (WindowsAPI::kWindow_height_ / 2) - (checkSize.y);
	checkPosition_.y = 0.0f;

	check_->Initialize(checkPosition_, checkSize);

	Vector2 aButtonSize = { 128.0f,128.0f };
	Vector2 aButtonPosition = { 0.0f,0.0f };
	aButtonPosition.x = (WindowsAPI::kWindow_width_ / 2) - (aButtonSize.x / 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y);
	aButton_->Initialize(aButtonPosition, aButtonSize);

	//�V�F�[�_�[�ǂݍ���
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();

	//�R���g���[��
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//�T�E���h
	Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("clear.wav");
	Sound::GetInstance()->PlaySoundWave("clear.wav",false);

	Sound::GetInstance()->LoadSoundWave("touch.wav");

	//�ϐ�
	move_ = {0.0f,0.0f};

	//�A���J�[�|�C���g�̐ݒ�
	Vector2 checkAnchorPoint = { 0.5f,0.5f };
	check_->SetAnchorPoint(checkAnchorPoint);
}

void ClearScene::Update()
{
	end_->matUpdate();
	aButton_->matUpdate();

	
	if(checkPosition_.y <= (WindowsAPI::kWindow_height_ / 2))
	{
		move_.y += 0.6f;
		checkPosition_.y += move_.y;
	}

	if(rotate_ <= ToRadian(360.0f))
	{
		float angle = ToRadian(360.0f);
		rotate_ -= PlayEaseIn(rotateTimer_, 0.0f, angle, kRotateTime_);
	}

	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else
	{
	}

	check_->SetPosition(checkPosition_);
	check_->SetRotation(rotate_);
	check_->matUpdate();

	//�Q�[���p�b�h���q�����Ă��邩�ǂ���
	if(gamePad_->IsConnected(Player1)) {}
	//�������u�Ԃ̔�������
	gamePad_->PushedButtonMoment();
	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		Sound::GetInstance()->PlaySoundWave("touch.wav",false);
		//SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}
}

void ClearScene::Draw()
{
	//�`��J�n
	directXBasic_->BeforeDraw();

	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	end_->Update();
	check_->Update();
	aButton_->Update();

	end_->Draw("WhiteTex");
	check_->Draw("check.png");
	aButton_->Draw("A.png");

	//�`��I��
	directXBasic_->AfterDraw();
}

void ClearScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;
}
