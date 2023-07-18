#include "MyFramework.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "xaudio2.h"
#include <wrl.h>

using namespace Microsoft::WRL;

void MyFramework::Initialize()
{
	//WindowsApi����������
	winApi_ = std::make_unique<WindowsAPI>();
	winApi_->Initialize();

	//DirectX����������
	directXBasic_ = std::make_unique<DirectXBasic>();
	directXBasic_->Initialize(winApi_.get());
	
	//Input����������
	input_ = std::make_unique<Input>();
	input_->Initialize(winApi_.get());

	//ImGui����������
	/*imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), directXBasic_.get());*/

	//�T�E���h����
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	
	//�X�v���C�g
	SpriteCommon::GetInstance()->Initialize(directXBasic_.get());
	Sprite::StaticInitialize();

	//�T�E���h
	Sound::GetInstance()->Initialize();

	//��ՃV�[���ÓI������
	//scene_ = std::make_unique<TitleScene>();
	//scene_->StaticInitialize(directXBasic_.get(), imGuiManager_.get());
	////�^�C�g���V�[������������
	//scene_->Initialize();

}

void MyFramework::Update()
{
	//windows���b�Z�[�W����
	if(winApi_->ProcessMessage())
	{
		//�I�����b�Z�[�W��������Q�[�����[�v�𔲂���t���O��ON�ɂ���
		endRequst_ = true;
	}

	input_->Update();
	//scene_->Update();
}

void MyFramework::Finalize()
{
	Sound::GetInstance()->Finalize();
	//�Q�[���S�̂̏I������
	winApi_->Finalize();
}

void MyFramework::Run()
{
	//�Q�[���̏�����
	Initialize();

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
	//�Q�[���̏I��
	Finalize();
}
