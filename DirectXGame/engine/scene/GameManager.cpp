#include "GameManager.h"
#include <wrl.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

GameManager::GameManager()
{
	//�Q�[���̏�����
	Initialize();
	//�Q�[�����[�v
	while(true)
	{
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if(IsEndRequst()){
			break;
		}
		//�`��
		Draw();
	}
	//�Q�[���̏I������
	Finalize();
}

void GameManager::Initialize()
{
	//WindowsApi����������
	winApi_ = std::make_unique<WindowsAPI>();
	winApi_->Initialize();

	//DirectX����������
	directXBasic_ = std::make_unique<DirectXBasic>();
	directXBasic_->Initialize(winApi_.get());

	//ImGui����������
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), directXBasic_.get());

	//�T�E���h����
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//Input����������
	input_ = std::make_unique<Input>();
	input_->Initialize(winApi_.get());

	//GameScene����������
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(directXBasic_.get(), imGuiManager_.get());

	//�|�X�g�G�t�F�N�g����������
	postEffect_ = std::make_unique<PostEffect>();
	//�|�X�g�G�t�F�N�g�p�e�N�X�`���̓ǂݍ���
	SpriteCommon::LoadTexture("test.png");
	postEffect_->Initialize(directXBasic_.get());
}

void GameManager::Update()
{
	//windows���b�Z�[�W����
	if(winApi_->ProcessMessage())
	{
		//�I�����b�Z�[�W��������Q�[�����[�v�𔲂���t���O��ON�ɂ���
		endRequst_ = true;
	}

	// DirectX���t���[������ ��������
	imGuiManager_->Begin();

	input_->Update();

	postEffect_->SetAnchorPoint(XMFLOAT2(0.0f, 0.0f));
	gameScene_->Update();

	imGuiManager_->End();
}

void GameManager::Draw()
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

void GameManager::Finalize()
{
	//�Q�[���S�̂̏I������
	imGuiManager_->Finalize();
	winApi_->Finalize();
}