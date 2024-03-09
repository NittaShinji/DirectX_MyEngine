#include "WindowsAPI.h"
#include <imgui_impl_win32.h>

#pragma comment(lib,"winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ウィンドウプロシージャ
LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ImGui用ウインドウプロシージャ呼び出し
	if(ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch(msg)
	{
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsAPI* WindowsAPI::GetInstance()
{
	static WindowsAPI insatnce;

	return &insatnce;
}

void WindowsAPI::Initialize()
{
	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w_.lpszClassName = L"Chameleon Ball"; // ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	//　ウインドウクラスをOSに登録する
	RegisterClassEx(&w_);
	// ウインドウサイズ{X座標　Y座標　横幅　縦幅}
	wrc_ = { 0,0,kWindow_width_,kWindow_height_ };
	//自動でサイズを補正する
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(w_.lpszClassName, // クラス名
		L"Chameleon Ball", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示Y座標(OSに任せる)
		wrc_.right - wrc_.left, // ウィンドウ横幅
		wrc_.bottom - wrc_.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w_.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);
}

//メッセージ処理
bool WindowsAPI::ProcessMessage()
{
	MSG msg{}; // メッセージ

	if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

void WindowsAPI::Finalize()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}
