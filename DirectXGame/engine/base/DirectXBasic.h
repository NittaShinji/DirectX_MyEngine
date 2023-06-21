#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include "WindowsAPI.h"

//DirectX���
class DirectXBasic
{
	
public:

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//������
	void Initialize(WindowsAPI* winApi);
	
	//�`��O����
	void BeforeDraw();
	//�`��㏈��
	void AfterDraw();

private:

	//FPS�Œ菉����
	void InitializeFixFPS();

	//FPS�Œ�X�V
	void UpdateFixFPS();

	std::chrono::steady_clock::time_point reference_;
	
	WindowsAPI* winApi_ = nullptr;

	HRESULT result_;

	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12Resource> depthBuff_;

	//�t�F���X
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	std::vector<ComPtr<ID3D12Resource>> backBuffers_;

	//���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//�������֐�
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapChain();
	void InitializeTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

public:	//�A�N�Z�b�T

	const HRESULT& GetResult() const { return result_; };

	ComPtr<ID3D12GraphicsCommandList> GetCommandList() const { return commandList_; };

	ComPtr<ID3D12Resource> GetDepthBuff() const { return depthBuff_; };

	ComPtr<ID3D12Device> GetDevice() const { return device_; };

	int32_t GetBackBuffersCount() const { return static_cast<int32_t>(backBuffers_.size()); };

	int32_t GetWinWidth() const { return winApi_->GetWinWidth(); }
	int32_t GetWinHeight() const { return winApi_->GetWinHeight(); }

	ComPtr<ID3D12CommandQueue> GetCommandQueue() { return commandQueue_; }
	ComPtr<ID3D12Fence> GetFence() { return fence_; }
};