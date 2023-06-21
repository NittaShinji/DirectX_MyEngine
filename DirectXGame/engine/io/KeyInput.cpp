#define DIRECTINPUT_VERSION  0x0800
#include "Input.h"
#include"KeyInput.h"
#include <cstdint>
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

KeyInput* KeyInput::instance = nullptr;
BYTE KeyInput::sKeys_[256];
BYTE KeyInput::sOldKeys_[256];
Microsoft::WRL::ComPtr<IDirectInputDevice8> KeyInput::keyboard_;

//アクセッサ
BYTE KeyInput::GetKeys(uint8_t keyNumber) const
{
	return sKeys_[keyNumber];
}

BYTE KeyInput::GetOldKeys(uint8_t oldKeyNumber) const
{
	return sOldKeys_[oldKeyNumber];
}

void KeyInput::Initialize(WindowsAPI* winApi)
{
	HRESULT result;

	//DirectInputの初期化
	result = DirectInput8Create(
		winApi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	//キーボードのデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		winApi->GetHwndClass(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyInput::KeyUpdate()
{
	KeyAssert();

	SaveFrameKey();

	//キーボード情報の取得開始
	keyboard_->Acquire();
	//全てのキーの入力情報を取得する
	keyboard_->GetDeviceState(sizeof(sKeys_), sKeys_);
}

void KeyInput::SaveFrameKey()
{
	//1フレーム前の情報を保存する
	for (int i = 0; i < 256; i++)
	{
		sOldKeys_[i] = sKeys_[i];
	}
}

#pragma region トリガー処理関数

//キーを押した状態か
bool KeyInput::HasPushedKey(BYTE keyNumber)
{
	KeyAssert();

	if (sKeys_[keyNumber] != 0 && sOldKeys_[keyNumber] != 0)
	{
		return true;
	}
	return false;
}

//キーを離した状態か
bool KeyInput::HasReleasedKey(BYTE keyNumber)
{
	KeyAssert();

	if (sKeys_[keyNumber] == 0 && sOldKeys_[keyNumber] == 0)
	{
		return true;
	}
	return false;
}

//キーを押した瞬間か
bool KeyInput::PushedKeyMoment(BYTE keyNumber)
{
	KeyAssert();

	if (sKeys_[keyNumber] != 0 && sOldKeys_[keyNumber] == 0)
	{
		return true;
	}
	return false;
}

//キーを離した瞬間か
bool KeyInput::ReleasedKeyMoment(BYTE keyNumber)
{
	KeyAssert();

	if (sKeys_[keyNumber] == 0 && sOldKeys_[keyNumber] != 0)
	{
		return true;
	}
	return false;
}

#pragma endregion 

void KeyInput::KeyAssert(){
	for (int32_t i = 0; i < 255; i++)
	{
		assert(SUCCEEDED(sKeys_[i]));
		assert(SUCCEEDED(sOldKeys_[i]));
	}
}
