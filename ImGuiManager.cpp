#include "ImGuiManager.h"

void ImGuiManager::Initialize(WindowsAPI* winApi, DirectXBasic* directXBasic)
{
	//WindowsAPI���Z�b�g
	winApi_ = winApi;
	//directXBasic���Z�b�g
	directXBasic_ = directXBasic;

	//ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();
	//ImGui�̃X�^�C����ݒ�
	ImGui::StyleColorsDark();

	//�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//�f�X�N���v�^�q�[�v����
	HRESULT result = directXBasic_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//Win32�p������
	ImGui_ImplWin32_Init(winApi_->GetHwndClass());
	//DirectX12�p������
	ImGui_ImplDX12_Init(directXBasic_->GetDevice().Get(), 
		static_cast<int>(directXBasic_->GetBackBuffersCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(), srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	//�W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();

}

void ImGuiManager::Finalize()
{
	//��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�f�X�N���v�^�����
	srvHeap_.Reset();
}

void ImGuiManager::Begin()
{
	//ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}


void ImGuiManager::End()
{
	ImGui::Render();
}


void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = directXBasic_->GetCommandList().Get();

	//�f�X�N���v�^�q�[�v�z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppheaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppheaps), ppheaps);
	//�`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
