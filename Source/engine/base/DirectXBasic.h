#pragma once
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include "WindowsAPI.h"
#pragma warning(push)
#pragma warning(disable:4820)
#include <d3d12.h>
#pragma warning(pop)

//DirectX基盤
class DirectXBasic
{

public:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//初期化
	void Initialize(WindowsAPI* winApi);

	//描画前処理
	void BeforeDraw();
	//描画後処理
	void AfterDraw();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();

private:

	//FPS固定初期化
	void InitializeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

	std::chrono::steady_clock::time_point reference_;

	WindowsAPI* winApi_ = nullptr;

	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12Resource> depthBuff_;

	//フェンス
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//初期化関数
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapChain();
	void InitializeTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

public:	//アクセッサ

	ComPtr<ID3D12GraphicsCommandList> GetCommandList() const { return commandList_; };

	ComPtr<ID3D12Resource> GetDepthBuff() const { return depthBuff_; };

	ComPtr<ID3D12Device> GetDevice() const { return device_; };

	int32_t GetBackBuffersCount() const { return static_cast<int32_t>(backBuffers_.size()); };

	int32_t GetWinWidth() const { return winApi_->GetWinWidth(); }
	int32_t GetWinHeight() const { return winApi_->GetWinHeight(); }

	ComPtr<ID3D12CommandQueue> GetCommandQueue() { return commandQueue_; }
	ComPtr<ID3D12Fence> GetFence() { return fence_; }
};