#define DIRECTINPUT_VERSION  0x0800
#include "Input.h"
#include"KeyInput.h"
#include <cstdint>
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

KeyInput* KeyInput::instance = nullptr;

//�A�N�Z�b�T
BYTE KeyInput::GetKeys(uint8_t keyNumber)
{
	return keys[keyNumber];
}

BYTE KeyInput::GetOldKeys(uint8_t oldKeyNumber)
{
	return oldKeys[oldKeyNumber];
}

void KeyInput::Initialize(WindowsAPI* winApi)
{
	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(
		winApi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�̃f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		winApi->GetHwndClass(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyInput::KeyUpdate()
{
	KeyAssert();

	/*GetKeys(keyNumber);
	GetOldKeys(keyNumber);*/
	SaveFrameKey();
	/*HasPushedKey(keyNumber);
	HasReleasedKey(keyNumber);
	PushedKeyMoment(keyNumber);
	ReleasedKeyMoment(keyNumber);*/
}

void KeyInput::SaveFrameKey()
{
	KeyAssert();

	//1�t���[���O�̏���ۑ�����
	for (int i = 0; i < 256; i++)
	{
		oldKeys[i] = keys[i];
	}

	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�S�ẴL�[�̓��͏����擾����
	keyboard->GetDeviceState(sizeof(keys), keys);
}

#pragma region �g���K�[�����֐�

//�L�[����������Ԃ�
bool KeyInput::HasPushedKey(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] != 0)
	{
		return true;
	}
	return false;
}

//�L�[�𗣂�����Ԃ�
bool KeyInput::HasReleasedKey(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] == 0 && oldKeys[keyNumber] == 0)
	{
		return true;
	}
	return false;
}

//�L�[���������u�Ԃ�
bool KeyInput::PushedKeyMoment(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] == 0)
	{
		return true;
	}
	return false;
}

//�L�[�𗣂����u�Ԃ�
bool KeyInput::ReleasedKeyMoment(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] == 0 && oldKeys[keyNumber] != 0)
	{
		return true;
	}
	return false;
}

#pragma endregion 

void KeyInput::KeyAssert(){
	for (int i = 0; i < 255; i++)
	{
		assert(SUCCEEDED(keys[i]));
		assert(SUCCEEDED(oldKeys[i]));
	}
}
