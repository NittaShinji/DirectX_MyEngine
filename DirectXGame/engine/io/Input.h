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

private:

	KeyInput* keyInput_ = nullptr;
	static const int32_t kKeyNumber_ = 256;
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput_ = nullptr;
};