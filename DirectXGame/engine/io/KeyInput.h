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

	//�ÓI�I�u�W�F�N�g�Ƃ��ăC���X�^���X�𐶐�
	static KeyInput* GetInstance()
	{
		static KeyInput instance;
		return &instance;
	}

	//�A�N�Z�b�T
	BYTE GetKeys(uint8_t keynumber) const ;
	BYTE GetOldKeys(uint8_t oldKeyNumber) const;

	//DirectInput�̏�����
	void Initialize(WindowsAPI* winApi);
	
	//�x���֐�
	static void KeyAssert();

	//�L�[�{�[�h�̍X�V
	static void KeyUpdate();
	//�O�t���[����ۑ�����֐�
	static void SaveFrameKey();

	//�g���K�[�����֐�
	//�L�[����������Ԃ�
	static bool HasPushedKey(BYTE keyNumber);
	//�L�[�𗣂�����Ԃ�
	static bool HasReleasedKey(BYTE keyNumber);
	//�L�[���������u�Ԃ�
	static bool PushedKeyMoment(BYTE keyNumber);
	//�L�[�𗣂����u�Ԃ�
	static bool ReleasedKeyMoment(BYTE keyNumber);

	//�f�o�C�X�������Ɏ��s�����
	//BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);
	
private:

	//�R���X�g���N�^
	KeyInput() = default;
	//�f�X�g���N�^
	~KeyInput() = default;
	//�C���X�^���X
	static KeyInput* instance;

	//�e��L�[�̕ۑ��p�ϐ�
	static BYTE sKeys_[256];
	static BYTE sOldKeys_[256];

	//�C���X�^���X
	ComPtr<IDirectInput8> directInput_ = nullptr;
	//�L�[�{�[�h�f�o�C�X
	static ComPtr<IDirectInputDevice8> keyboard_;
	//�R���g���[���[
	static ComPtr<IDirectInputDevice8> gamePad_;


};