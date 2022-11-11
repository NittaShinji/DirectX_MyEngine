#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>
#include "WindowsAPI.h"

//DirectX基盤
class DirectXBasic
{
public:

	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//初期化
	void Initialize(WindowsAPI* winApi);
	
	/*DirectXBasic();
	~DirectXBasic();*/

	//描画前処理
	void BeforeDraw();
	//描画後処理
	void AfterDraw();



	HRESULT GetResult() { return result_; };

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList_; };

	Microsoft::WRL::ComPtr<ID3D12Resource> GetDepthBuff() { return depthBuff_; };

	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const { return device_; };
	
private:

	
	WindowsAPI* winApi_ = nullptr;
	//std::unique_ptr<WindowsAPI> winApi_ = std::make_unique<WindowsAPI>();

	HRESULT result_;

	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

	/*ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12GraphicsCommandList> commandList_;*/

	//ComPtr<ID3D12Resource> depthBuff_;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
	
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc{};

	//フェンス
	//ComPtr<ID3D12Fence> fence;

	UINT64 fenceVal = 0;

	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapChain();
	void InitializeTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

};