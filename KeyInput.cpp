#define DIRECTINPUT_VERSION  0x0800
#include "Input.h"
#include"KeyInput.h"
#include <cstdint>
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

KeyInput* KeyInput::instance = nullptr;

//アクセッサ
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

	//DirectInputの初期化
	result = DirectInput8Create(
		winApi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードのデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		winApi->GetHwndClass(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyInput::KeyUpdate(BYTE keyNumber)
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

	//1フレーム前の情報を保存する
	for (int i = 0; i < 256; i++)
	{
		oldKeys[i] = keys[i];
	}

	//キーボード情報の取得開始
	keyboard->Acquire();
	//全てのキーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(keys), keys);
}

#pragma region トリガー処理関数

//キーを押した状態か
bool KeyInput::HasPushedKey(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] != 0)
	{
		return true;
	}
	return false;
}

//キーを離した状態か
bool KeyInput::HasReleasedKey(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] == 0 && oldKeys[keyNumber] == 0)
	{
		return true;
	}
	return false;
}

//キーを押した瞬間か
bool KeyInput::PushedKeyMoment(BYTE keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] == 0)
	{
		return true;
	}
	return false;
}

//キーを離した瞬間か
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
