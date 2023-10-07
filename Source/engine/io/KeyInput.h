#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <cstdint>
#include <wrl.h>
#pragma warning(push)
#pragma warning(disable:4820)
#include <dinput.h>
#pragma warning(pop)

class KeyInput
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//コピーコンストラクタ
	KeyInput(const KeyInput&) = delete;
	//コピー代入演算子
	KeyInput& operator=(const KeyInput&) = delete;
	//ムーブコンストラクタ
	KeyInput(KeyInput&&) = delete;
	//ムーブ代入演算子
	KeyInput& operator = (KeyInput&&) = delete;

	//静的オブジェクトとしてインスタンスを生成
	static KeyInput* GetInstance()
	{
		static KeyInput instance;
		return &instance;
	}

	//アクセッサ
	BYTE GetKeys(uint8_t keynumber) const;
	BYTE GetOldKeys(uint8_t oldKeyNumber) const;

	//DirectInputの初期化
	void Initialize(WindowsAPI* winApi);

	//警告関数
	static void KeyAssert();

	//キーボードの更新
	static void KeyUpdate();
	//前フレームを保存する関数
	static void SaveFrameKey();

	//トリガー処理関数
	//キーを押した状態か
	static bool HasPushedKey(BYTE keyNumber);
	//キーを離した状態か
	static bool HasReleasedKey(BYTE keyNumber);
	//キーを押した瞬間か
	static bool PushedKeyMoment(BYTE keyNumber);
	//キーを離した瞬間か
	static bool ReleasedKeyMoment(BYTE keyNumber);

private:

	//コンストラクタ
	KeyInput() = default;
	//デストラクタ
	~KeyInput() = default;
	//インスタンス
	static KeyInput* instance;

	//各種キーの保存用変数
	static BYTE sKeys_[256];
	static BYTE sOldKeys_[256];

	//インスタンス
	ComPtr<IDirectInput8> directInput_ = nullptr;
	//キーボードデバイス
	static ComPtr<IDirectInputDevice8> keyboard_;
};