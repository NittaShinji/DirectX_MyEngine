#include "StageSelect.h"
#include "SceneManager.h"
#include "Sound.h"
#include "WindowsAPI.h"
#include "Vector2.h"
#include "Easing.h"

DirectXBasic* StageSelectScene::directXBasic_ = nullptr;
ImGuiManager* StageSelectScene::imGuiManager_ = nullptr;

void StageSelectScene::StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager)
{
	directXBasic_ = BaseScene::directXBasic_;
	imGuiManager_ = BaseScene::imGuiManager_;

	Sprite::StaticInitialize();
}

void StageSelectScene::Initialize()
{
	gamePad_ = std::make_unique<GamePad>();
	gamePad_->Initialzie(Player1);

	//�摜
	selectSprite_ = std::make_unique<Sprite>();
	BackGroundLeft = std::make_unique<Sprite>();
	
	SpriteCommon::GetInstance()->LoadTexture("StageSelect.png");
	SpriteCommon::GetInstance()->LoadTexture("SelectBackGround.png");
	
	Vector2 selectPosition = { 0.0f,0.0f };
	const Vector2 selectSize = { 1280.0f,720.0f };
	selectSprite_->Initialize(selectPosition, selectSize);

	Vector2 backGroundPosition = { 40.0f,180.0f };
	const Vector2 backGroundSize = { 600.0f,360.0f };
	BackGroundLeft->Initialize(backGroundPosition, backGroundSize);

	//�V�F�[�_�[�ǂݍ���
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
	//�T�E���h
	Sound::GetInstance()->Initialize();
	//Sound::GetInstance()->LoadSoundWave("title.wav");
	Sound::GetInstance()->LoadSoundWave("touch.wav");

	//Sound::GetInstance()->PlaySoundWave("title.wav");
}

void StageSelectScene::Update()
{
	//�摜�̍X�V
	selectSprite_->matUpdate();
	BackGroundLeft->matUpdate();

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
		changeSceneTimer_--;
		if(changeSceneTimer_ <= 0)
		{
			Sound::GetInstance()->Finalize();
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}

void StageSelectScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	selectSprite_->Draw("StageSelect.png");
	BackGroundLeft->Draw("SelectBackGround.png");

}