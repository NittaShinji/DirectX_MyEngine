#include "TitleScene.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	title_ = std::make_unique<Sprite>();
	SpriteCommon::GetInstance()->LoadTexture("title.png");
	XMFLOAT2 titlePosition = { 0,0 };
	XMFLOAT2 titleSize = { 1280,720 };
	title_->Initialize(titlePosition, titleSize);
	//�V�F�[�_�[�ǂݍ���
	SpriteCommon::GetInstance()->ShaderLoad();
	SpriteCommon::GetInstance()->SemiTransparent();
}

void TitleScene::Update()
{
	//�A���J�[�|�C���g�̐ݒ�
	XMFLOAT2 anchorPoint = { 0.0f,0.0f };
	title_->SetAnchorPoint(anchorPoint);
	title_->matUpdate();

	if(keys_->HasPushedKey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	SpriteCommon::GetInstance()->BeforeDraw();
	SpriteCommon::GetInstance()->Update();
	title_->Draw("title.png");
}

void TitleScene::Finalize()
{
}