#pragma once
#include <Windows.h>


//windowsAPI
class WindowsAPI
{
public:
	//初期化
	void Initialize();
	
	//静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//ゲッター
	WNDCLASSEX GetWindosClass() const { return w;}
	HWND GetHwndClass()const{ return hwnd;}
	RECT GetWrcClass() const {return wrc;}
	HINSTANCE GetHInstance()const { return w.hInstance;}
	int GetWinWidth() const { return window_width;}
	int GetWinHeight() const { return window_height;}

	//終了
	void Finalize();

	//メッセージの処理
	bool ProcessMessage();

	//定数
	//ウインドウサイズ
	static constexpr int window_width = 1280; //横幅
	static constexpr int window_height = 720; //縦幅

private:

	// ウィンドウ
	WNDCLASSEX w{};
	// ウィンドウオブジェクト
	HWND hwnd;
	// ウインドウサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc;
};