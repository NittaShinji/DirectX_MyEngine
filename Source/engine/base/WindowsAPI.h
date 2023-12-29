#pragma once
#include <Windows.h>
#include <stdint.h>

//windowsAPI
class WindowsAPI final
{
public:

	//コピーコンストラクタを無効にする
	WindowsAPI(const WindowsAPI& windowsApi) = delete;
	//代入演算子を無効
	WindowsAPI& operator=(const WindowsAPI& windowsApi) = delete;

	//インスタンスを返す
	static WindowsAPI* GetInstance();

	//初期化
	void Initialize();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//終了
	void Finalize();

	//メッセージの処理
	bool ProcessMessage();

public:

	//定数
	//ウインドウサイズ
	static constexpr int32_t kWindow_width_ = 1280; //横幅
	static constexpr int32_t kWindow_height_ = 720; //縦幅

private:

	//コンストラクタ
	WindowsAPI() {};
	//デストラクタ
	~WindowsAPI() {};

private:

	// ウィンドウ
	WNDCLASSEX w_{};
	// ウィンドウオブジェクト
	HWND hwnd_;
	// ウインドウサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc_;

public:

	//ゲッター

	//ウインドウを取得
	const WNDCLASSEX& GetWindosClass() const { return w_; }
	const HINSTANCE& GetHInstance()const { return w_.hInstance; }
	//ウィンドウオブジェクトを取得
	const HWND& GetHwndClass()const { return hwnd_; }
	//ウインドウサイズ{ X座標　Y座標　横幅　縦幅 }を取得
	const RECT& GetWrcClass() const { return wrc_; }
	//ウインドウの横幅を取得
	int32_t GetWinWidth() const { return kWindow_width_; }
	//ウインドウの縦幅を取得
	int32_t GetWinHeight() const { return kWindow_height_; }
};