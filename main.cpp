#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
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


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region WindowsAPI初期化処理
	//①ゲーム全体の初期化
	//ウインドウサイズ
	const int window_width = 1280; //横幅
	const int window_height = 720; //縦幅

	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	//　ウインドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウインドウサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(w.lpszClassName, // クラス名
		L"DirectXGame", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示Y座標(OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション
		// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	MSG msg{}; // メッセージ
#pragma endregion WindowsAPI初期化処理
#pragma region DirectX初期化処理
	//DirectX初期化処理 ここから

	#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
	#endif

	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
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
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));
	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
	commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
	(IDXGISwapChain1**)&swapChain);
	assert(SUCCEEDED(result));

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);

	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}

	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	 
	//DirectX初期化処理 ここまで
#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	// 頂点データ
	XMFLOAT3 vertices[] = {
	{ -0.5f, -0.5f, 0.0f }, // 左下
	{ -0.5f, +0.5f, 0.0f }, // 左上
	{ +0.5f, -0.5f, 0.0f }, // 右下
	};
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{
	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}, // (1行で書いたほうが見やすい)
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));


#pragma endregion 
	// ゲームループ
	while (true) {

#pragma region ウインドウメッセージ処理
		//ゲームを実行している間、繰り返し行う処理
		// ブロック内はページ右側を参照
		// メッセージがある?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // プロシージャにメッセージを送る
		}
		// ×ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT) {
			break;
		}
#pragma endregion ウインドウメッセージ処理
#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理 ここから
		 // バックバッファの番号を取得(2つなので0番か1番)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex]; // バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		commandList->ResourceBarrier(1, &barrierDesc);

		// 2.描画先の変更
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		 
		// 3.画面クリア R G B A
		FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma region グラフィックコマンド
		// 4.描画コマンドここから
		 // ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = window_width;
		viewport.Height = window_height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		// ビューポート設定コマンドを、コマンドリストに積む
		commandList->RSSetViewports(1, &viewport);

		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + window_width; // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + window_height; // 切り抜き座標下
		// シザー矩形設定コマンドを、コマンドリストに積む
		commandList->RSSetScissorRects(1, &scissorRect);

		// パイプラインステートとルートシグネチャの設定コマンド
		commandList->SetPipelineState(pipelineState);
		commandList->SetGraphicsRootSignature(rootSignature);

		// プリミティブ形状の設定コマンド
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		commandList->IASetVertexBuffers(0, 1, &vbView);

		// 描画コマンド
		commandList->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
#pragma endregion グラフィックコマンド
		// 4.描画コマンドここまで

		// 5.リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
		commandList->ResourceBarrier(1, &barrierDesc);
		// DirectX毎フレーム処理 ここまで

#pragma endregion  DirectX毎フレーム処理

#pragma region 画面入れ替え

		// 命令のクローズ
		result = commandList->Close();
		assert(SUCCEEDED(result));
		// コマンドリストの実行
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);
		// 画面に表示するバッファをフリップ(裏表の入替え)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
		// コマンドの実行完了を待つ
		commandQueue->Signal(fence, ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		// キューをクリア
		result = commandAllocator->Reset();
		assert(SUCCEEDED(result));
		// 再びコマンドリストを貯める準備
		result = commandList->Reset(commandAllocator, nullptr);
		assert(SUCCEEDED(result));
#pragma endregion 画面入れ替え
		
	}

#pragma region WindowsAPI後始末
	//③ゲーム全体の終了処理
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

#pragma endregion WindowsAPI後始末
	return 0;

}