
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
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform
{
	XMMATRIX mat;	// 3D�ϊ��s��
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

	//DirectX����������
	directXBasic->Initialize(winApi);

	Model::StaticInitialize(directXBasic);

	//�V���O���g���C���X�^���X���쐬
	KeyInput::Create();
	KeyInput* keyInput = KeyInput::GetInstance();

	input->Initialize(winApi);

	//3D�I�u�W�F�N�g�ÓI������
	//Object3d::StaticInitialize(directXBasic->GetDevice(), directXBasic->GetWinWidth(), directXBasic->GetWinHeight());
	//�X�v���C�g����������
	Sprite* sprite = nullptr;
	sprite = new Sprite;
	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;

	spriteCommon->Initialize(directXBasic);
	sprite->Initialize(spriteCommon);
	spriteCommon->ShaderLoad();

	sprite->LoadTexture(0,"reimu.png");
	sprite->LoadTexture(1,"tomas.png");


	//3D�I�u�W�F�N�g����
	Object3d* object3d = nullptr;
	object3d = new Object3d("Resources/triangle/triangle.obj",directXBasic,sprite);


	//Sprite* testSprite = nullptr;
	//testSprite = new Sprite;

	///*Model* model = nullptr;
	//model = new Model(directXBasic);*/


	/*sprite->TexMapping();
	sprite->TexMappingSRVSet();*/
	/*sprite->ImageDateSet();
	sprite->ImageDateSRVSet();*/
	//sprite->LoadTexture(0,"tomas.png");
	//testSprite->LoadTexture(1, "reimu.png");
	//spriteCommon->SemiTransparent();

#pragma region �L�[���� (P02_03)

	
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

		//XMFLOAT2 position = sprite->GetPosition();
		////���W��ύX����
		//position.x += 0.1f;
		////�ύX�𔽉f
		//sprite->SetPosition(position);

		//float rotation = sprite->GetRotation();
		////�p�x��ύX����
		//rotation += 0.001f;
		////�ύX�𔽉f
		//sprite->SetRotation(rotation);

		//XMFLOAT4 color = sprite->GetColor();
		//color = { 0,0,1,1 };
		//sprite->SetColor(color);

		//XMFLOAT2 anchorPoint = sprite->GetAnchorPoint();
		//anchorPoint = { 1.0f,1.0f };
		//sprite->SetAnchorPoint(anchorPoint);

		sprite->matUpdate();
		//testSprite->matUpdate();

		object3d->Update();

#pragma endregion

		directXBasic->BeforeDraw();

#pragma region �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h(P02_01)

#pragma endregion

#pragma region SRV�ݒ�R�}���h

		//object3d->Draw();

#pragma endregion 
		//spriteCommon->SetSRVheap(sprite->GetSRVheap());
		/*spriteCommon->Update();*/

		//�X�v���C�g�`��J�n
		//spriteCommon->BeforeDraw();
		//sprite->Draw();
		////�X�v���C�g�`��I��
		//spriteCommon->AfterDraw();
		//

		object3d->BeforeDraw();

		object3d->Draw(sprite->GetSRVHeap());

		object3d->AfterDraw();

		//testSprite->Draw();

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
	//delete testSprite;
	delete object3d;
#pragma endregion WindowsAPI��n��

	winApi = nullptr;
	directXBasic = nullptr;
	//input = nullptr;
	return 0;
}