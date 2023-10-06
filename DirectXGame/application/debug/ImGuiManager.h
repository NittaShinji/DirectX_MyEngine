#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

//ImGuiの管理
class ImGuiManager
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WindowsAPI* winApi, DirectXBasic* directXBasic);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw();

private:

	DirectXBasic* directXBasic_ = nullptr;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

};