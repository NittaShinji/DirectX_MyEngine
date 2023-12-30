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

/// <summary>
/// DirectX基盤
/// </summary>
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

private: //メンバ関数

	//FPS固定初期化
	void InitializeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

	//初期化関数
	//デバイスを初期化
	void InitializeDevice();
	//コマンドを初期化
	void InitializeCommand();
	//スワップチェインを初期化
	void InitializeSwapChain();
	//レンダーターゲットビューの初期化
	void InitializeTargetView();
	//デプスバッファの初期化
	void InitializeDepthBuffer();
	//フェンスの初期化
	void InitializeFence();

private: //メンバ変数

	std::chrono::steady_clock::time_point reference_;

	WindowsAPI* winApi_ = nullptr;

	//デバイス
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	//スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	//コマンドアロケーター
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	//コマンドキュー
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	//コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	//デプスバッファ
	ComPtr<ID3D12Resource> depthBuff_;
	//フェンス
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	//デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	
public:	//アクセッサ

	//コマンドリストを取得
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() const { return commandList_; };
	//デプスバッファを取得
	ComPtr<ID3D12Resource> GetDepthBuff() const { return depthBuff_; };
	//デバイスを取得
	ComPtr<ID3D12Device> GetDevice() const { return device_; };
	//バックバッファのサイズを取得
	int32_t GetBackBuffersCount() const { return static_cast<int32_t>(backBuffers_.size()); };
	//ウインドウの横幅を取得
	int32_t GetWinWidth() const { return winApi_->GetWinWidth(); }
	//ウインドウの縦幅を取得
	int32_t GetWinHeight() const { return winApi_->GetWinHeight(); }
	//コマンドキューを取得
	ComPtr<ID3D12CommandQueue> GetCommandQueue() { return commandQueue_; }
	//フェンスを取得
	ComPtr<ID3D12Fence> GetFence() { return fence_; }
};