#include "MyGame.h"
#include "SceneFactory.h"
#include "StageSelect.h"
#include "TutorialScene.h"
#include "ClearScene.h"

#include "Vector4.h"
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
	StageSelectScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	TutorialScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	ClearScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
}

void MyGame::Update()
{
	// DirectX���t���[������ ��������
	imGuiManager_->Begin();

	//���N���X�̍X�V����
	MyFramework::Update();

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//�Q�[���V�[���̕`��
	SceneManager::GetInstance()->Draw();
}

void MyGame::Finalize()
{
	//�Q�[���S�̂̏I������
	imGuiManager_->Finalize();

	//���N���X�̏I������
	MyFramework::Finalize();
}