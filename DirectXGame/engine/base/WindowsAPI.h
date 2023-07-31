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

	static WindowsAPI* GetInstance();

	//初期化
	void Initialize();
	
	//静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//ゲッター
	const WNDCLASSEX& GetWindosClass() const { return w_;}
	const HWND& GetHwndClass()const{ return hwnd_;}
	const RECT& GetWrcClass() const {return wrc_;}
	const HINSTANCE& GetHInstance()const { return w_.hInstance;}
	int32_t GetWinWidth() const { return kWindow_width_;}
	int32_t GetWinHeight() const { return kWindow_height_;}

	//終了
	void Finalize();

	//メッセージの処理
	bool ProcessMessage();

	//定数
	//ウインドウサイズ
	static constexpr int32_t kWindow_width_ = 1280; //横幅
	static constexpr int32_t kWindow_height_ = 720; //縦幅

private:

	WindowsAPI() {};
	~WindowsAPI() {};

	// ウィンドウ
	WNDCLASSEX w_{};
	// ウィンドウオブジェクト
	HWND hwnd_;
	// ウインドウサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc_;
};