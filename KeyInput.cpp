#define DIRECTINPUT_VERSION  0x0800
#include"KeyInput.h"
#include <cstdint>
#include <cassert>
#include <wrl.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

KeyInput* KeyInput::instance = nullptr;

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//�C���X�^���X
ComPtr<IDirectInput8> directInput = nullptr;
//�L�[�{�[�h�f�o�C�X
ComPtr<IDirectInputDevice8> keyboard = nullptr;

//�A�N�Z�b�T
BYTE KeyInput::GetKeys(uint8_t keyNumber)
{
	return keys[keyNumber];
}

BYTE KeyInput::GetOldKeys(uint8_t oldKeyNumber)
{
	return oldKeys[oldKeyNumber];
}

void KeyInput::Initialize(HINSTANCE windowHinstance,HWND hwnd )
{
	HRESULT result;

	//DirectInput�̏�����
	result = DirectInput8Create(
		windowHinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�̃f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyInput::SaveFrameKey()
{
	KeyAssert();

	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//1�t���[���O�̏���ۑ�����
	for (int i = 0; i < 256; i++)
	{
		oldKeys[i] = keys[i];
	}

	//�S�ẴL�[�̓��͏����擾����
	keyboard->GetDeviceState(sizeof(keys), keys);
}

void KeyInput::KeyUpdate()
{
	//HasPushedKey;
}

//�L�[����������Ԃ�
bool KeyInput::HasPushedKey(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] != 0)
	{
		return TRUE;
	}
}

//�L�[�𗣂�����Ԃ�
bool KeyInput::HasReleasedKey(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] == 0 && oldKeys[keyNumber] == 0)
	{
		return TRUE;
	}
}

//�L�[���������u�Ԃ�
bool KeyInput::PushedKeyMoment(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] == 0)
	{
		return TRUE;
	}
}

//�L�[�𗣂����u�Ԃ�
bool KeyInput::ReleasedKeyMoment(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] == 0 && oldKeys[keyNumber] != 0)
	{
		return TRUE;
	}
}

void KeyInput::KeyAssert()
{}
