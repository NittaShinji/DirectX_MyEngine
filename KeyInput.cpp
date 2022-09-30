#define DIRECTINPUT_VERSION  0x0800
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

void KeyInput::Initialize(HINSTANCE windowHinstance,HWND hwnd )
{
	HRESULT result;

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		windowHinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードのデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyInput::SaveFrameKey()
{
	KeyAssert();

	//キーボード情報の取得開始
	keyboard->Acquire();

	//1フレーム前の情報を保存する
	for (int i = 0; i < 256; i++)
	{
		oldKeys[i] = keys[i];
	}

	//全てのキーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(keys), keys);

}

//キーを押した状態か
bool KeyInput::HasPushedKey(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] != 0)
	{
		return TRUE;
	}
}

//キーを離した状態か
bool KeyInput::HasReleasedKey(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] == 0 && oldKeys[keyNumber] == 0)
	{
		return TRUE;
	}
}

//キーを押した瞬間か
bool KeyInput::PushedKeyMoment(int keyNumber)
{
	KeyAssert();

	if (keys[keyNumber] != 0 && oldKeys[keyNumber] == 0)
	{
		return TRUE;
	}
}

//キーを離した瞬間か
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
