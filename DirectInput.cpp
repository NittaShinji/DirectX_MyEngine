#include"DirectInput.h"
#include <cstdint>
#include <cassert>

//コンストラクタ
KeyInput::KeyInput()
{}

//デストラクタ
KeyInput::~KeyInput()
{}

//アクセッサ
uint8_t KeyInput::GetReadKey()
{
	return *keys;
}

uint8_t KeyInput::GetReadOldKey()
{
	return *oldkeys;
}

void KeyInput::Initialize()
{
}

void KeyInput::SaveFrameKey(uint8_t* keys, uint8_t* oldkeys)
{
	assert(SUCCEEDED(keys != nullptr));
	assert(SUCCEEDED(oldkeys != nullptr));

	keyboard->Acquire();


	keyboard->GetDeviceState(sizeof(keys), keys);


	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
	}
}

//キーを押した状態か
bool KeyInput::HasPushedKey(uint8_t* keys, uint8_t* oldkeys)
{
	assert(SUCCEEDED(keys != nullptr));
	assert(SUCCEEDED(oldkeys != nullptr));

	if (keys != 0 && oldkeys != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//キーを離した状態か
bool KeyInput::HasReleasedKey(uint8_t* keys, uint8_t* oldkeys)
{
	assert(SUCCEEDED(keys != nullptr));
	assert(SUCCEEDED(oldkeys != nullptr));

	if (keys == 0 && oldkeys == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//キーを押した瞬間か
bool KeyInput::PushedKeyMoment(uint8_t* keys, uint8_t* oldkeys)
{
	assert(SUCCEEDED(keys != nullptr));
	assert(SUCCEEDED(oldkeys != nullptr));

	if (keys != 0 && oldkeys == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//キーを離した瞬間か
bool KeyInput::ReleasedKeyMoment(uint8_t* keys, uint8_t* oldkeys)
{
	assert(SUCCEEDED(keys != nullptr));
	assert(SUCCEEDED(oldkeys != nullptr));

	if (keys == 0 && oldkeys != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
