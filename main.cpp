
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
#include <memory>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform
{
	XMMATRIX mat;	// 3D�ϊ��s��
};

struct Object3d
{
	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform;

	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matworld;
	//�e�I�u�W�F�N�g�̃|�C���^
	Object3d* parent = nullptr;

};

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
	directXBasic->Initialize(winApi);

	SpriteCommon* spriteCommon = nullptr;
	//�X�v���C�g���ʕ���
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(directXBasic);

	//�Q�[���S�̂̏�����
	Sprite* sprite = nullptr;
	sprite = new Sprite;
	sprite->Initialize(spriteCommon);
	
#pragma region �L�[���� (P02_03)

	//�V���O���g���C���X�^���X���쐬
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);

#pragma endregion

#pragma region �}�e���A���p�̒萔�o�b�t�@�̏�����(P03_02)
	
#pragma endregion

#pragma region �}�e���A���p�̒萔�o�b�t�@�̓]��(P03_02)
	
#pragma endregion

#pragma region �O�����p�̒萔�o�b�t�@�̏�����(P05_02)
	
#pragma endregion

#pragma region �O�����p�̒萔�o�b�t�@�̓]��(P05_02)
	
#pragma endregion

#pragma region ��ڂ̃I�u�W�F�N�g�p�̒萔�o�b�t�@
	
#pragma endregion	

#pragma	region ���_�f�[�^(P02_01)
	
#pragma endregion 

#pragma region ���_�o�b�t�@�̊m��(P02_01)

	
#pragma endregion 

#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��(P03_04)
	
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬(P03_04)
	
#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̍쐬(P02_01)
	
#pragma endregion
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��(P02_01)

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��(P02_01)
	
#pragma endregion

#pragma region ���_���C�A�E�g(P02_01)
	
#pragma endregion

#pragma region �摜�ǂݍ���
	
#pragma endregion

#pragma region �摜�ǂݍ��ݓ񖇖�

#pragma endregion

#pragma region �摜�ǂݍ��ݎO����

#pragma endregion

#pragma region �摜�C���[�W�f�[�^�̍쐬(������)

#pragma endregion 

#pragma region �e�N�X�`���o�b�t�@�̐ݒ�(P04_02)

#pragma endregion

#pragma region �񖇖ڂ̃e�N�X�`���o�b�t�@�̐ݒ�(P04_02)

#pragma endregion

#pragma region �O���ڂ̃e�N�X�`���o�b�t�@�̐ݒ�(P04_02)
	
#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�̐���(P04_02)
	
#pragma endregion

#pragma region �񖇖ڂ̃e�N�X�`���o�b�t�@�̐���(P04_02)
	
#pragma endregion

#pragma region �O���ڂ̃e�N�X�`���o�b�t�@�̐���(P04_02)
	
#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�Ƀf�[�^�]��(P04_03_27)

#pragma endregion

#pragma region �f�X�N���v�^�q�[�v����(04_02)

#pragma endregion
#pragma region �f�X�N���v�^�n���h��
	
#pragma endregion

#pragma region �[�x�r���[�p�̃f�X�N���v�^�q�[�v���쐬

#pragma endregion

#pragma region �[�x�X�e���V���r���[�̐���

#pragma endregion

#pragma region �V�F�[�_�[���\�[�X�r���[�ݒ�(P04_03)
	
#pragma endregion

#pragma region �񖇖ڂ̉摜�p�V�F�[�_�[���\�[�X�r���[�ݒ�(P06_06)

	
#pragma endregion

#pragma region �O���ڂ̉摜�p�V�F�[�_�[���\�[�X�r���[�ݒ�(P06_06)

	
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�(P02_02_P04)
	
#pragma region �u�����h�ݒ�(03_01)
	
#pragma endregion 	
#pragma region �f�X�N���v�^�����W�̐ݒ�

	
#pragma endregion 
#pragma region ���[�g�p�����[�^�̐ݒ�(P03_02)(05_02_P16) 
	
#pragma endregion 
#pragma region ���[�g�V�O�l�`���̐ݒ�(P02_01_P38)

	
#pragma endregion 

#pragma endregion 

	// �Q�[�����[�v
	while (true) {

#pragma region �E�C���h�E���b�Z�[�W����
		//�Q�[�������s���Ă���ԁA�J��Ԃ��s������
		// �u���b�N���̓y�[�W�E�����Q��
		// ���b�Z�[�W������?
		//windows���b�Z�[�W����
		if (winApi->ProcessMessage())
		{
			//�I�����b�Z�[�W��������Q�[�����[�v�𔲂���
			break;
		}
#pragma endregion �E�C���h�E���b�Z�[�W����
#pragma region DirectX���t���[������
		// DirectX���t���[������ ��������
		input->Update();
		
#pragma region ���_�o�b�t�@�ւ̃f�[�^�]�� (P02_01)
	
#pragma endregion

#pragma endregion

#pragma region ���[���h�ϊ��s�� 

#pragma endregion

		directXBasic->BeforeDraw();

#pragma region �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h(P02_01)

#pragma endregion

#pragma region SRV�ݒ�R�}���h

		
#pragma endregion 

		spriteCommon->Update();
		sprite->Update();
		
#pragma region ���̑��̐ݒ�R�}���h

		directXBasic->AfterDraw();
	}

#pragma endregion

#pragma region WindowsAPI��n��
	//�Q�[���S�̂̏I������
	
	//input->Delete();
	//KeyInput::destroy();
	//delete keyInput;
	delete winApi;
	delete directXBasic;
	delete spriteCommon;
	//delete playerSprite;
	delete sprite;
#pragma endregion WindowsAPI��n��

	winApi = nullptr;
	directXBasic = nullptr;
	//input = nullptr;
	return 0;
}