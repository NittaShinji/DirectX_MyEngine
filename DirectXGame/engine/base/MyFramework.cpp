#include "MyFramework.h"
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

	//�T�E���h����
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);
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
}

void MyFramework::Finalize()
{
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
