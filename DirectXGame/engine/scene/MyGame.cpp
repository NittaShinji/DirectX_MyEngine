#include "MyGame.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

MyGame::MyGame(){}

void MyGame::Initialize()
{
	//���N���X�̏���������
	MyFramework::Initialize();

	//ImGui����������
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), directXBasic_.get());

	//GameScene����������
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(directXBasic_.get(), imGuiManager_.get());

	//�|�X�g�G�t�F�N�g����������
	postEffect_ = std::make_unique<PostEffect>();
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���̓ǂݍ���
	SpriteCommon::LoadTexture("test.png");
	postEffect_->Initialize(directXBasic_.get());
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	MyFramework::Update();

	postEffect_->SetAnchorPoint(XMFLOAT2(0.0f, 0.0f));

	// DirectX���t���[������ ��������
	imGuiManager_->Begin();

	gameScene_->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//�����_�[�e�N�X�`���̕`��
	postEffect_->PreDrawScene();
	gameScene_->Draw();
	postEffect_->PostDrawScene();

	//�`��J�n
	directXBasic_->BeforeDraw();
	//�|�X�g�G�t�F�N�g�̕`��
	postEffect_->Draw("test.png");
	imGuiManager_->Draw();

	//�`��I��
	directXBasic_->AfterDraw();
}

void MyGame::Finalize()
{
	//�Q�[���S�̂̏I������
	imGuiManager_->Finalize();

	//���N���X�̏I������
	MyFramework::Finalize();
}