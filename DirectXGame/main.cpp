
//#include <d3d12.h>
//#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
using namespace DirectX;
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include "Input.h"
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "GameScene.h"
#include "xaudio2.h"
#include "ImGuiManager.h"
#include "PostEffect.h"
#include <memory>
#include <fstream>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"xaudio2.lib")

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	//WindowsApi����������
	std::unique_ptr<WindowsAPI> winApi = std::make_unique<WindowsAPI>();
	winApi->Initialize();

	//DirectX����������
	std::unique_ptr<DirectXBasic> directXBasic = std::make_unique<DirectXBasic>();
	directXBasic->Initialize(winApi.get());

	//ImGui����������
	std::unique_ptr<ImGuiManager> imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApi.get(), directXBasic.get());

	//------�T�E���h����

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//------���͏���
	std::unique_ptr<Input> input = std::make_unique<Input>();
	input->Initialize(winApi.get());

	//�Q�[���V�[��
	std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	gameScene->Initialize(directXBasic.get(), imGuiManager.get());

	PostEffect* postEffect = nullptr;

	//�|�X�g�G�t�F�N�g�p�e�N�X�`���̓ǂݍ���
	SpriteCommon::LoadTexture("test.png");
	postEffect = new PostEffect;
	postEffect->Initialize(directXBasic.get());
	
	// �Q�[�����[�v
	while (true) {
		//windows���b�Z�[�W����
		if (winApi->ProcessMessage())
		{
			//�I�����b�Z�[�W��������Q�[�����[�v�𔲂���
			break;
		}

		// DirectX���t���[������ ��������

		imGuiManager->Begin();

		input->Update();

		postEffect->SetAnchorPoint(XMFLOAT2(0.0f, 0.0f));
		/*postEffect->matUpdate();*/
		gameScene->Update();

		imGuiManager->End();

		//�����_�[�e�N�X�`���̕`��
		//postEffect->PreDrawScene();
		//postEffect->PostDrawScene();

		//�`��J�n
		directXBasic->BeforeDraw();
		gameScene->Draw();
		//�|�X�g�G�t�F�N�g�̕`��
		//postEffect->Draw("test.png");
		imGuiManager->Draw();
		
		//�`��I��
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI��n��

	//�Q�[���S�̂̏I������
	imGuiManager->Finalize();
	winApi->Finalize();

	delete postEffect;
	
#pragma endregion WindowsAPI��n��

	return 0;
}