#include "TitleScene.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"

DirectXBasic* TitleScene::directXBasic_ = nullptr;
ImGuiManager* TitleScene::imGuiManager_ = nullptr;

void TitleScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	Sprite::StaticInitialize();
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);
	Mesh::StaticInitialize(directXBasic_);
	LightGroup::StaticInitialize(directXBasic_->GetDevice().Get());
	Camera::StaticInitialize(directXBasic_);
}

void TitleScene::Initialize()
{
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	lightGroup_ = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup_);

	//3D�I�u�W�F�N�g
	titleSphere_ = Object3d::Create("sphere");
	spherPos_.x = 0.0f;
	spherPos_.y = 5.0f;
	spherPos_.z = 5.0f;

	titleSphere_->SetTransform(spherPos_);
	titleSphere_->SetScale(Vector3{ 3.0f, 3.0f, 3.0f });

	//�摜
	titleSprite_ = std::make_unique<Sprite>();
	clickSprite_= std::make_unique<Sprite>();
	aButtonSprite_ = std::make_unique<Sprite>();
	bButtonSprite_ = std::make_unique<Sprite>();

	SpriteCommon::GetInstance()->LoadTexture("title.png");
	SpriteCommon::GetInstance()->LoadTexture("A.png");
	SpriteCommon::GetInstance()->LoadTexture("B.png");
	SpriteCommon::GetInstance()->LoadTexture("click.png");

	Vector2 titlePosition = { 0.0f,0.0f };
	const Vector2 titleSize = { 1280.0f,720.0f };
	titleSprite_->Initialize(titlePosition, titleSize);

	const Vector2 clickButtonSize = { 128.0f,128.0f };
	Vector2 clickButtonPosition;
	clickButtonPosition.x = (WindowsAPI::kWindow_width_  / 2) - (clickButtonSize.x / 2);
	clickButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (clickButtonSize.y) + (clickButtonSize.y / 3);

	const Vector2 aButtonSize = { 128.0f,128.0f };
	Vector2 aButtonPosition;
	aButtonPosition.x = (WindowsAPI::kWindow_width_) - (aButtonSize.x * 2);
	aButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (aButtonSize.y) + (aButtonSize.y / 3);

	const Vector2 bButtonSize = { 128.0f,128.0f };
	Vector2 bButtonPosition;
	bButtonPosition.x = (WindowsAPI::kWindow_width_) - (bButtonSize.x);
	bButtonPosition.y = (WindowsAPI::kWindow_height_ / 2) + (bButtonSize.y) + (bButtonSize.y / 3);

	clickSprite_->Initialize(clickButtonPosition, clickButtonSize);
	aButtonSprite_->Initialize(aButtonPosition, aButtonSize);
	bButtonSprite_->Initialize(bButtonPosition, bButtonSize);

	//�V�F�[�_�[�ǂݍ���
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//�T�E���h
	Sound::GetInstance()->Initialize();
	Sound::GetInstance()->LoadSoundWave("title.wav");
	Sound::GetInstance()->LoadSoundWave("touch.wav");

	Sound::GetInstance()->PlaySoundWave("title.wav");

	//�J����
	camera_ = std::make_unique<Camera>();

	Vector3 cameraEye = { 30,15.5,-20 };

	Vector3 cameraTarget = { 0,5,5 };
	
	Vector3 cameraUp = { 0,1,0 };

	camera_->Initialize(cameraEye, cameraTarget, cameraUp);

	isChangeScene_ = false;
	isChangeColor_ = false;
	rotateAcc_ = 0.0f;
	moveRotate_ = 0.0f;

	bool isJump = false;
	Vector3 jumpVec = {0.0f,0.0f,0.0f};
	float jumpAcc = 0.0f;
}

void TitleScene::Update()
{
	camera_->Update();

	//�A���J�[�|�C���g�̐ݒ�
	Vector2 anchorPoint = { 0.0f,0.0f };
	titleSprite_->SetAnchorPoint(anchorPoint);
	titleSprite_->matUpdate();
	clickSprite_->SetAnchorPoint(anchorPoint);
	clickSprite_->matUpdate();
	aButtonSprite_->SetAnchorPoint(anchorPoint);
	aButtonSprite_->matUpdate();
	bButtonSprite_->SetAnchorPoint(anchorPoint);
	bButtonSprite_->matUpdate();

	titleSphere_->Update(camera_.get());

	//��]����
	if(isChangeScene_ == false)
	{
		//sphereRotate.y -= 0.01f;
		//rotateAcc_ -= PlayEaseIn(rotateTimer_, 0.0f, 1.0f, kRotateTime_);
		//sphereRotate.y += rotateAcc_;
		float angle = ToRadian(360.0f);
		sphereRotate.y -= PlayEaseIn(rotateTimer_, 0.0f, angle, kRotateTime_);
		titleSphere_->SetRotation(sphereRotate);
	}

	/*if(rotateTimer_ >= kRotateTime_)
	{
		rotateTimer_ = 0.0f;
		rotateAcc_ = 0.0f;
	}
	else
	{
		rotateTimer_++;
	}*/

	if(rotateTimer_ >= 0)
	{
		rotateTimer_--;
	}
	else
	{
		rotateTimer_ = kRotateTime_;

		isJump = true;
		moveTimer_ = kActionTime_;

		if(isChangeColor_ == false)
		{
			isChangeColor_ = true;
		}
		else
		{
			isChangeColor_ = false;
		}
	}

	if(isJump == true)
	{
		////�����������x�@
		//const float fallAcc = -0.015f;
		//const float fallVYMin = -0.5f;
		////����
		//move_.y = max(move_.y + fallAcc, fallVYMin);
		////�ړ�
		//spherPos_.x += move_.x;
		//spherPos_.y += move_.y;
		//spherPos_.z += move_.z;


		//jumpAcc += PlayEaseIn(moveTimer_, 0.0, 1.0, kActionTime_);
		//spherPos_.y += jumpAcc;
		spherPos_.y += PlayEaseIn(moveTimer_, 0.0, 1.0, kActionTime_);

		//spherPos_.y += 1.0f;

		titleSphere_->SetTransform(spherPos_);
	}
	
	//�ړ�����
	if(moveTimer_ > 0)
	{
		moveTimer_--;
	}
	else
	{
		isJump = false;
	}

	if(isJump == false)
	{
		if(spherPos_.y >= 5.0f)
		{
			spherPos_.y -= easeInCubic(0.75f);
		}
		titleSphere_->SetTransform(spherPos_);
	}

	if(moveTimer_ > (kActionTime_ / 2))
	{
		isDown_ = false;
		isUp_ = true;
		move_.y = 0;
	}
	else
	{
		isDown_ = true;
		isUp_ = false;
		move_.y = 0;
	}

	//�F��ς��鏈��
	if(changeColorTimer_ >= 0)
	{
		changeColorTimer_--;

	}
	else
	{
		changeColorTimer_ = kChangeColorTime_;
	}

	//if(changeColorTimer >= 0)
	//{
	//	changeColorTimer -= PlayEaseIn(rotateTimer_, 0.0f, 1.0f, kRoateTime_);
	//}
	//else
	//{
	//	changeColorTimer = kRoateTime_;
	//	if(isChangeColor_ == true)
	//	{
	//		isChangeColor_ = false;
	//	}
	//	else
	//	{
	//		isChangeColor_ = true;
	//	}
	//}
	

	if(isChangeColor_ == false)
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 0.4f, 0.7f));
	}
	else
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 0.469f, 0.0f));
	}
	/*if(changeColorTimer_ > (kChangeColorTime_ / 2))
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 0.4f, 0.7f));
	}
	else
	{
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 0.469f, 0.0f));
	}*/



	if(isDown_ == true && isUp_ == false)
	{
		move_.y -= 0.3f;
	}
	else if(isUp_ == true && isDown_ == false)
	{
		move_.y += 0.3f;
	}

	titleSphere_->SetTransform(spherPos_);

	//�Q�[���p�b�h���q�����Ă��邩�ǂ���
	if(gamePad_->IsConnected(Player1)) {}
	//�������u�Ԃ̔�������
	gamePad_->PushedButtonMoment();

	if(gamePad_->GetButtonA() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		Sound::GetInstance()->PlaySoundWave("touch.wav");
		isChangeScene_ = true;	
	}

	if(isChangeScene_ == true)
	{
		changeWhiteTimer_--;
		titleSphere_->SetColorFlag(true);
		titleSphere_->SetColor(Vector3(1.0f, 1.0f, 1.0f));
		if(changeWhiteTimer_ <= 0)
		{
			Sound::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}

void TitleScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	titleSprite_->Draw("title.png");
	clickSprite_->Draw("click.png");
	aButtonSprite_->Draw("A.png");
	bButtonSprite_->Draw("B.png");


	Object3d::BeforeDraw();
	titleSphere_->BeforeDraw();
	titleSphere_->Draw();
}