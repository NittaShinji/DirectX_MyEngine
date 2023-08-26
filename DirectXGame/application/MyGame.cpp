#include "MyGame.h"
#include "SceneFactory.h"
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
	SceneManager::GetInstance()->ChangeScene("GAME");
	//��x�̂ݏ�����
	TitleScene::StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	GameScene::StaticInitialize(directXBasic_.get(),imGuiManager_.get());

	//�|�X�g�G�t�F�N�g����������
	postEffect_ = std::make_unique<PostEffect>();
	
	postEffect_->Initialize(directXBasic_.get());

	//�w�i�I�u�W�F�N�g�̃e�X�g�p�ɉ��J������z�u
	Vector3 cameraEye = { 30,7.5,-20 };
	Vector3 cameraTarget = { 0,5,5 };
	Vector3 cameraUp = { 0,1,0 };
	testCamera_ = std::make_unique<Camera>();
	testCamera_->Initialize(cameraEye, cameraTarget, cameraUp);

	//�u���[�p�̔w�i�I�u�W�F�N�g
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();
}

void MyGame::Update()
{
	// DirectX���t���[������ ��������
	imGuiManager_->Begin();

	//���N���X�̍X�V����
	MyFramework::Update();

	postEffect_->SetAnchorPoint(Vector2(0.0f, 0.0f));

	//�J�����A�w�i�I�u�W�F�N�g�̍X�V
	testCamera_->Update();
	backGround_->Update(testCamera_.get());

	SceneManager::GetInstance()->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	//�����_�[�e�N�X�`���̕`��
	postEffect_->PreDrawScene();
	//�w�i�I�u�W�F�N�g�̕`��
	Object3d::BeforeDraw();
	backGround_->Draw();
	postEffect_->PostDrawScene();

	//�`��J�n
	directXBasic_->BeforeDraw();

	//�|�X�g�G�t�F�N�g�̕`��
	postEffect_->Draw();

	//�[�x�l�N���A
	directXBasic_->ClearDepthBuffer();
	
	//�Q�[���V�[���̕`��
	SceneManager::GetInstance()->Draw();

	//�f�o�b�O�e�L�X�g�̕`��
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