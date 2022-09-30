#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <cstdint>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class KeyInput
{
public:
	
	//コピーコンストラクタ
	KeyInput(const KeyInput&) = delete;
	//コピー代入演算子
	KeyInput& operator=(const KeyInput&) = delete;
	//ムーブコンストラクタ
	KeyInput(KeyInput&&) = delete;
	//ムーブ代入演算子
	KeyInput& operator = (KeyInput&&) = delete;

	//インスタンスを参照
	static KeyInput* GetInstance()
	{
		return instance;
	}

	//インスタンスの生成
	static void Create()
	{
		if (!instance)
		{
			instance = new KeyInput;
		}
	}

	//インスタンスの破棄
	static void destroy()
	{
		delete instance;
		instance = nullptr;
	}

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

	//キーボードの更新
	void KeyUpdate();

private:

	//コンストラクタ
	KeyInput() = default;
	//デストラクタ
	~KeyInput() = default;
	//インスタンス
	static KeyInput* instance;

	//各種キーの保存用変数
	BYTE keys[256] = {};
	BYTE oldKeys[256] = {};

	//キーボードデバイス
	IDirectInputDevice8* keyboard = nullptr;
};

//KeyInput* KeyInput::instance = nullptr;