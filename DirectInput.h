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
	uint8_t GetReadKey();
	uint8_t GetReadOldKey();
	void Initialize();

	//前フレームを保存する関数
	void  SaveFrameKey(uint8_t* keys, uint8_t *oldkeys);
	
	//トリガー処理関数
	bool HasPushedKey(uint8_t *keys, uint8_t *oldkeys);
	bool HasReleasedKey(uint8_t *keys, uint8_t *oldkeys);
	bool PushedKeyMoment(uint8_t *keys, uint8_t *oldkeys);
	bool ReleasedKeyMoment(uint8_t *keys, uint8_t *oldkeys);

private:

	BYTE keys[256];
	BYTE oldkeys[256];
};

