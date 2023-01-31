#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

//ImGui�̊Ǘ�
class ImGuiManager
{

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WindowsAPI* winApi, DirectXBasic* directXBasic);

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui��t�J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui��t�I��
	/// </summary>
	void End();

	/// <summary>
	/// ��ʂւ̕`��
	/// </summary>
	void Draw();

private :

	WindowsAPI* winApi_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

};

