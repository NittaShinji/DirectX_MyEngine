#include "ImGuiManager.h"

void ImGuiManager::Initialize(WindowsAPI* winApi, DirectXBasic* directXBasic)
{
	//directXBasicをセット
	directXBasic_ = directXBasic;

	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//デスクリプタヒープ生成
	HRESULT result;
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//Win32用初期化
	ImGui_ImplWin32_Init(winApi->GetHwndClass());
	//DirectX12用初期化
	ImGui_ImplDX12_Init(directXBasic_->GetDevice().Get(),
		directXBasic_->GetBackBuffersCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(), srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	//標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//デスクリプタを解放
	srvHeap_.Reset();
}

void ImGuiManager::Begin()
{
	//ImGuiフレーム開始
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

	//デスクリプタヒープ配列をセットするコマンド
	ID3D12DescriptorHeap* ppheaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppheaps), ppheaps);
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}