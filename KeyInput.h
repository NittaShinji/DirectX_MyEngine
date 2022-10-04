#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <cstdint>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class KeyInput
{
public:
	
	//�R�s�[�R���X�g���N�^
	KeyInput(const KeyInput&) = delete;
	//�R�s�[������Z�q
	KeyInput& operator=(const KeyInput&) = delete;
	//���[�u�R���X�g���N�^
	KeyInput(KeyInput&&) = delete;
	//���[�u������Z�q
	KeyInput& operator = (KeyInput&&) = delete;

	//�C���X�^���X���Q��
	static KeyInput* GetInstance()
	{
		return instance;
	}

	//�C���X�^���X�̐���
	static void Create()
	{
		if (!instance)
		{
			instance = new KeyInput;
		}
	}

	//�C���X�^���X�̔j��
	static void destroy()
	{
		delete instance;
		instance = nullptr;
	}

	//�A�N�Z�b�T
	BYTE GetKeys(uint8_t keynumber);
	BYTE GetOldKeys(uint8_t oldKeyNumber);

	//DirectInput�̏�����
	void Initialize(HINSTANCE windowHinstance, HWND hwnd);

	//�O�t���[����ۑ�����֐�
	void SaveFrameKey();

	//�x���֐�
	void KeyAssert();

	//�g���K�[�����֐�
	//�L�[����������Ԃ�
	bool HasPushedKey(int keyNumber);
	//�L�[�𗣂�����Ԃ�
	bool HasReleasedKey(int keyNumber);
	//�L�[���������u�Ԃ�
	bool PushedKeyMoment(int keyNumber);
	//�L�[�𗣂����u�Ԃ�
	bool ReleasedKeyMoment(int keyNumber);

	//�L�[�{�[�h�̍X�V
	void KeyUpdate();

private:

	//�R���X�g���N�^
	KeyInput() = default;
	//�f�X�g���N�^
	~KeyInput() = default;
	//�C���X�^���X
	static KeyInput* instance;

	//�e��L�[�̕ۑ��p�ϐ�
	BYTE keys[256] = {};
	BYTE oldKeys[256] = {};

	//IDirectInputDevice8* keyboard = nullptr;
};