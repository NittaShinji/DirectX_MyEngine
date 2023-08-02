#include "MyGame.h"
#include "SceneFactory.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

MyGame::MyGame(){}

void MyGame::Initialize()
{
	//���N���X�̏���������
	MyFramework::Initialize();
	
	//�Q�[���V�[������������
	//�V�[���t�@�N�g���[�𐶐����A�}�l�[�W���ɃZ�b�g
	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(std::move(sceneFactory_));
	//�V�[���}�l�[�W���ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->ChangeScene("TITLE");
	//��x�̂ݏ�����
	TitleScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());

	//�|�X�g�G�t�F�N�g����������
	postEffect_ = std::make_unique<PostEffect>();
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���̓ǂݍ���
	SpriteCommon::GetInstance()->LoadTexture("postEffect.png");
	postEffect_->Initialize(directXBasic_.get());

}

void MyGame::Update()
{
	// DirectX���t���[������ ��������
	imGuiManager_->Begin();

	//���N���X�̍X�V����
	MyFramework::Update();

	postEffect_->SetAnchorPoint(Vector2(0.0f, 0.0f));

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//�����_�[�e�N�X�`���̕`��
	postEffect_->PreDrawScene();
	SceneManager::GetInstance()->Draw();
	postEffect_->PostDrawScene();

	//�`��J�n
	directXBasic_->BeforeDraw();
	//�|�X�g�G�t�F�N�g�̕`��
	postEffect_->Draw("postEffect.png");
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