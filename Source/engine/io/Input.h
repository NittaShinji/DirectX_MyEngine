#pragma once
#include <cstdint>
#include <wrl.h>
#include <memory>
#include "WindowsAPI.h"
#include "KeyInput.h"

/// <summary>
/// 入力クラス
/// </summary>
class Input
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//コンストラクタ
	Input();
	//デストラクタ
	~Input();

	//初期化
	void Initialize(WindowsAPI* winApi);
	//更新
	void Update();

private:

	KeyInput* keyInput_ = nullptr;
	//キー番号
	static const int32_t kKeyNumber_ = 256;
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput_ = nullptr;
};