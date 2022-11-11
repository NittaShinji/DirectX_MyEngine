#include "DirectXBasic.h"
#include <cassert>
#include <vector>
#include <string>
#include <thread>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//DirectXBasic::DirectXBasic() {}
//DirectXBasic::~DirectXBasic() {}

using namespace Microsoft::WRL;

void DirectXBasic::Initialize(WindowsAPI* winApi)
{
	//NULLチェック
	assert(winApi);

	//メンバ変数にコピー
	this->winApi_ = winApi;

	//FPS固定
	InitializeFixFPS();

	//デバイス
	InitializeDevice();
	//コマンド関連
	InitializeCommand();
	//スワップチェーン
	InitializeSwapChain();
	//レンダーターゲートビュー(RTV)
	InitializeTargetView();
	//深度バッファ
	InitializeDepthBuffer();
	//フェンス
	InitializeFence();


#pragma region スワップチェーン (P01_02)
#pragma endregion 
#pragma region デスクリプタヒープの生成 (P01_02)
#pragma endregion
#pragma region バックバッファ(P01_02)
#pragma endregion 
#pragma region レンダーターゲートビュー(RTV)の生成 (P01_02)
#pragma endregion 
#pragma region 深度バッファのリソース設定
#pragma endregion 
#pragma region フェンス (P01_02)
#pragma endregion 
}

void DirectXBasic::BeforeDraw()
{
		// バックバッファの番号を取得(2つなので0番か1番)
		UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		
		barrierDesc.Transition.pResource = backBuffers_[bbIndex].Get(); // バックバッファを指定
		//barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定

		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		commandList_->ResourceBarrier(1, &barrierDesc);

#pragma endregion 

#pragma region 描画先指定コマンド (P01_03)
		// 2.描画先の変更
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

		//深度ステンシルビュー用のデスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
		commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion

#pragma region 画面クリアコマンド・背景色変更(P01_03)

		// 3.画面クリア R G B A
		FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f }; // 青っぽい色

		//背景色を変更
		//keyInput->ChangeColor(clearColor);
		//if (keyInput->HasPushedKey(DIK_SPACE))
		//{
		//	//画面クリアカラーの数値を書き換える
		//	clearColor[0] = 1.0f;
		//	clearColor[1] = 0.1f;
		//}

		commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
#pragma region グラフィックコマンド
		// 4.描画コマンドここから

#pragma region ビューポート設定コマンド

		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport;

		//1個目のビューポートを設定 左上
		viewport.Width = winApi_->GetWinWidth();      //横幅
		viewport.Height = winApi_->GetWinHeight();	//縦幅
		viewport.TopLeftX = 0;					//左上X
		viewport.TopLeftY = 0;					//左上Y
		viewport.MinDepth = 0.0f;				//最大深度
		viewport.MaxDepth = 1.0f;				//最小深度

		// ビューポート設定コマンドを、コマンドリストに積む
		commandList_->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region シザー矩形の設定コマンド

		// シザー矩形
		D3D12_RECT scissorRect;
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + winApi_->GetWinWidth(); // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + winApi_->GetWinHeight(); // 切り抜き座標下

		// シザー矩形設定コマンドを、コマンドリストに積む
		commandList_->RSSetScissorRects(1, &scissorRect);

}

void DirectXBasic::AfterDraw()
{
	// バックバッファの番号を取得(2つなので0番か1番)
		UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// 5.リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
		commandList_->ResourceBarrier(1, &barrierDesc);
#pragma endregion
		// DirectX毎フレーム処理 ここまで
#pragma endregion  DirectX毎フレーム処理

#pragma region 画面入れ替え

#pragma region コマンドのフラッシュ(P01_03)

		// 命令のクローズ
		result_ = commandList_->Close();
		assert(SUCCEEDED(result_));
		// コマンドリストの実行
		ID3D12CommandList* commandLists[] = { commandList_.Get()};
		commandQueue_->ExecuteCommandLists(1, commandLists);
		// 画面に表示するバッファをフリップ(裏表の入替え)
		result_ = swapChain_->Present(1, 0);
		assert(SUCCEEDED(result_));
#pragma endregion 

#pragma region コマンド完了待ち(P01_03)
		// コマンドの実行完了を待つ
		commandQueue_->Signal(fence.Get(), ++fenceVal);

		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		// キューをクリア
		result_ = commandAllocator_->Reset();
		assert(SUCCEEDED(result_));
		// 再びコマンドリストを貯める準備
		result_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
		assert(SUCCEEDED(result_));


		//FPS固定
		UpdateFixFPS();
}

void DirectXBasic::InitializeDevice()
{
#pragma region アダプタの列挙(P01_02)

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	/*ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableSynchronizedCommandQueueValidation(TRUE);
	}*/
#endif

	// DXGIファクトリーの生成
	result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result_));
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}
#pragma endregion

#pragma region デバイスの生成(P01_02)

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result_ = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result_ == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->Release();
	}
#endif
}

void DirectXBasic::InitializeCommand()
{
#pragma region コマンドリストをGPUにまとめて命令を送るため (P01_02)

	// コマンドアロケータを生成
		result_ = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result_));
	// コマンドリストを生成
	result_ = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region コマンドキューの生成(P01_02)
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	//コマンドキューを生成
	result_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result_));

#pragma endregion 
}

void DirectXBasic::InitializeSwapChain()
{
	// スワップチェーンの設定
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChainのComPtrを用意(初期化でのみ仕様)
	ComPtr<IDXGISwapChain1> swapChain1;

	// スワップチェーンの生成
	result_ = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApi_->GetHwndClass(), &swapChainDesc, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result_));

	//生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapChain1.As(&swapChain_);
}

void DirectXBasic::InitializeTargetView()
{
	// デスクリプタヒープの設定
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc_.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));

	backBuffers_.resize(swapChainDesc.BufferCount);

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers_.size(); i++) {

		// スワップチェーンからバッファを取得
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
		//device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void DirectXBasic::InitializeDepthBuffer()
{
#pragma region	深度バッファのリソース設定

	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApi_->GetWinWidth();								//レンダーターゲットに合わせる
	depthResourceDesc.Height = winApi_->GetWinHeight();							//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;					//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;		//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

#pragma endregion 

#pragma region 深度バッファ生成
	//深度バッファ生成
	result_ = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);
#pragma endregion

#pragma region 深度ビュー用のデスクリプタヒープを作成

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	//ID3D12DescriptorHeap* dsvHeap = nullptr;
	
	// デスクリプタヒープの作成
	result_ = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

#pragma endregion

#pragma region 深度ステンシルビューの生成

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);

#pragma endregion
}

void DirectXBasic::InitializeFence()
{
	// フェンスの生成
	
	result_ = device_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

}

void DirectXBasic::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void DirectXBasic::UpdateFixFPS()
{
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));

	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));
	//現在時間を取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回記録からの経過時間を記録
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60秒経っていない場合
	if (elapsed < kMinCheckTime)
	{
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	//現在時間を記録する
	reference_ = std::chrono::steady_clock::now();

}