
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
#include <memory>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();
	WindowsAPI* winApi = nullptr;
	winApi = new WindowsAPI();
	winApi->Initialize();

	std::unique_ptr<Input> input = std::make_unique<Input>();
	//std::unique_ptr<DirectXBasic> directBasic_ = std::make_unique<DirectXBasic>();
	DirectXBasic* directXBasic = nullptr;
	directXBasic = new DirectXBasic();

	//DirectX����������
	directXBasic->Initialize(winApi);

	Model::StaticInitialize(directXBasic);

	//�V���O���g���C���X�^���X���쐬
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);

	//3D�I�u�W�F�N�g����
	Object3d* object3d = nullptr;
	object3d = new Object3d("Resources/triangle/triangle.obj",directXBasic,0, "tomas.png");
	//object3d->LoadTexture(0, "tomas.png");

	// �Q�[�����[�v
	while (true) {
		//windows���b�Z�[�W����
		if (winApi->ProcessMessage())
		{
			//�I�����b�Z�[�W��������Q�[�����[�v�𔲂���
			break;
		}

		// DirectX���t���[������ ��������
		input->Update();

		object3d->Update();

		directXBasic->BeforeDraw();
		object3d->BeforeDraw();

		object3d->Draw();

		object3d->AfterDraw();
		directXBasic->AfterDraw();
	}

#pragma region WindowsAPI��n��
	//�Q�[���S�̂̏I������

	delete winApi;
	delete directXBasic;
	delete object3d;
#pragma endregion WindowsAPI��n��

	winApi = nullptr;
	directXBasic = nullptr;
	//input = nullptr;
	return 0;
}