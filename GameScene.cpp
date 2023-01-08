#include "GameScene.h"
#include <string>

using namespace std;
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//�摜
	delete spriteCommon_;
	delete title_;
	//���f��
	delete object3d_;
	delete nObject3d_;
	delete sObject3d_;

}

void GameScene::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	//------------�摜�ǂݍ���----------
	title_ = new Sprite;
	spriteCommon_ = new SpriteCommon;
	//�X�v���C�g�֌W������
	spriteCommon_->Initialize(directXBasic_);
	Sprite::StaticInitialize(spriteCommon_);

	//�摜�ǂݍ���
	Sprite::LoadTexture(0, "reimu.png");
	Sprite::LoadTexture(1, "tomas.png");

	//�X�̉摜��������(�w�肵���ԍ��̉摜���g�p����)
	XMFLOAT2 titlePosition = { 400,400 };
	XMFLOAT2 titleSize = { 100,100 };
	title_->Initialize(titlePosition, titleSize,1);

	//�V�F�[�_�[�ǂݍ���
	spriteCommon_->ShaderLoad();
	//�X�v���C�g�p�̃p�C�v���C���X�e�[�g(���߉\)��p��
	spriteCommon_->SemiTransparent();

	//------------���f���ǂݍ���----------
	Model::StaticInitialize(directXBasic_);
	Object3d::StaticInitialize(directXBasic_);

	//���f���ǂݍ���
	const string testModelName = "triangle_tex";
	const string testModelName2 = "triangle_tex2";
	const string testModelName3 = "pillar";

	Model::Load(testModelName);
	Model::Load(testModelName2);
	Model::Load(testModelName3);

	//3D�I�u�W�F�N�g�̐���
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 position2 = { 30,0,0 };
	XMFLOAT3 position3 = { -30,0,0 };

	object3d_ = new Object3d(testModelName, position);
	nObject3d_ = new Object3d(testModelName2, position2);
	sObject3d_ = new Object3d(testModelName3, position3);
}

void GameScene::Update()
{
	//���f���̍X�V����
	object3d_->Update();
	nObject3d_->Update();
	sObject3d_->Update();

	//�摜�̍X�V����
	//�A���J�[�|�C���g�̐ݒ�
	XMFLOAT2 anchorPoint = { 0.0f,0.0f };
	title_->SetAnchorPoint(anchorPoint);
	title_->matUpdate();
}

void GameScene::Draw()
{
	//���f���`��
	object3d_->BeforeDraw();
	object3d_->Draw();
	nObject3d_->Draw();
	sObject3d_->Draw();

	//�摜�`��
	spriteCommon_->BeforeDraw();
	spriteCommon_->Update();
	title_->Draw();
}
