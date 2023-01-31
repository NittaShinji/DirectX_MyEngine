
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
#include <memory>
#include <fstream>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"xaudio2.lib")

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();
	
	WindowsAPI* winApi = nullptr;
	winApi = new WindowsAPI();
	winApi->Initialize();

	
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	std::unique_ptr<Input> input = std::make_unique<Input>();
	//std::unique_ptr<DirectXBasic> directBasic_ = std::make_unique<DirectXBasic>();
	DirectXBasic* directXBasic = nullptr;
	directXBasic = new DirectXBasic();

	//DirectX����������
	directXBasic->Initialize(winApi);

	//ImGui����������
	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApi,directXBasic);


	//------�T�E���h

	//XAudio2�G���W���̃C���X�^���X�𐶐�
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�̐���
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//�V���O���g���C���X�^���X���쐬
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);
	
	GameScene* gameScene = nullptr;
	gameScene = new GameScene;
	gameScene->Initialize(directXBasic,imGuiManager);

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

		gameScene->Update();

		imGuiManager->End();

		//�`��
		directXBasic->BeforeDraw();
		
		gameScene->Draw();
		imGuiManager->Draw();
		
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI��n��

	keyInput->destroy();
	//�Q�[���S�̂̏I������
	imGuiManager->Finalize();
	winApi->Finalize();
	delete gameScene;
	delete imGuiManager;
	delete winApi;
	delete directXBasic;
	
#pragma endregion WindowsAPI��n��

	keyInput = nullptr;
	imGuiManager = nullptr;
	winApi = nullptr;
	directXBasic = nullptr;
	gameScene = nullptr;
	
	return 0;
}