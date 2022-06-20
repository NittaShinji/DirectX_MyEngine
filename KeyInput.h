#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <cstdint>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class KeyInput 
{
public:
	KeyInput();
	~KeyInput();

	//アクセッサ
	BYTE GetKeys(uint8_t keynumber);
	BYTE GetOldKeys(uint8_t oldKeyNumber);

	//DirectInputの初期化
	void Initialize(HINSTANCE windowHinstance, HWND hwnd);

	//前フレームを保存する関数
	void SaveFrameKey();

	//警告関数
	void KeyAssert();

	//トリガー処理関数
	//キーを押した状態か
	bool HasPushedKey(int keyNumber);
	//キーを離した状態か
	bool HasReleasedKey(int keyNumber);
	//キーを押した瞬間か
	bool PushedKeyMoment(int keyNumber);
	//キーを離した瞬間か
	bool ReleasedKeyMoment(int keyNumber);

private:

	BYTE keys[256] = {};
	BYTE oldKeys[256] = {};

	IDirectInputDevice8* keyboard;
};

