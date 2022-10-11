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
	HWND GetHwndClass() { return hwnd; }
	WNDCLASSEX GetWindosClass() { return w;}
	RECT GetWrcClass() {return wrc;}
	int GetWinWidth() { return window_width; }
	int GetWinHeight() { return window_height; }

	//メッセージの処理
	bool ProcessMessage();

private:

	// ウィンドウ
	WNDCLASSEX w{};
	// ウィンドウオブジェクト
	HWND hwnd;
	// ウインドウサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc;
	//ウインドウサイズ
	const int window_width = 1280; //横幅
	const int window_height = 720; //縦幅

};