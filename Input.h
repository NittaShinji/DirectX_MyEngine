#pragma once
#include <cstdint>
#include <wrl.h>
#include <memory>
#include "WindowsAPI.h"
#include "KeyInput.h"

class Input
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Input();
	~Input();

	void Initialize(WindowsAPI* winApi);
	void Update();
	void Delete();

private:

	KeyInput* keyInput_ = nullptr;
	static const int keyNumber_ = 256;
	BYTE keys_[keyNumber_] = {};
	BYTE oldKeys_[keyNumber_] = {};
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput = nullptr;
};