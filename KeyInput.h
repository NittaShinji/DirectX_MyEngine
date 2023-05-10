#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <cstdint>
#include <wrl.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class KeyInput
{
public:
	
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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
	void Initialize(WindowsAPI* winApi);
	
	//�x���֐�
	void KeyAssert();

	//�L�[�{�[�h�̍X�V
	void KeyUpdate();
	//�O�t���[����ۑ�����֐�
	void SaveFrameKey();

	//�g���K�[�����֐�
	//�L�[����������Ԃ�
	bool HasPushedKey(BYTE keyNumber);
	//�L�[�𗣂�����Ԃ�
	bool HasReleasedKey(BYTE keyNumber);
	//�L�[���������u�Ԃ�
	bool PushedKeyMoment(BYTE keyNumber);
	//�L�[�𗣂����u�Ԃ�
	bool ReleasedKeyMoment(BYTE keyNumber);
	
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

	//�C���X�^���X
	ComPtr<IDirectInput8> directInput = nullptr;
	//�L�[�{�[�h�f�o�C�X
	ComPtr<IDirectInputDevice8> keyboard = nullptr;
};