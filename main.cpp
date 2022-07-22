#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
using namespace DirectX;
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include <dinput.h>
#include "KeyInput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


void CreateConstantBuffer();
long ConstantBufferResult(long result, ID3D12Device* device, D3D12_HEAP_PROPERTIES cbHeapProp,
	D3D12_RESOURCE_DESC cbResourceDesc, ID3D12Resource* constBuffTransform);

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform
{
	XMMATRIX mat;	// 3D変換行列
};

struct Object3d
{
	//定数バッファ(行列用)
	ID3D12Resource* constBuffTransform;
	//定数バッファマップ(行列用)
	ConstBufferDataTransform* constMapTransform;

	//アフィン変換情報
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matworld;
	//親オブジェクトのポインタ
	Object3d* parent = nullptr;

};

void InitializeObject3d(Object3d* object, ID3D12Device* device);
void UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matProjection);
void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);

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
#pragma region ウインドウの設定 (P01_01)
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
#pragma endregion 

#pragma region ウインドウの生成 (P01_01)
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

#pragma endregion 

	MSG msg{}; // メッセージ
#pragma endregion WindowsAPI初期化処理
#pragma region DirectX初期化処理
	//DirectX初期化処理 ここから

#ifdef _DEBUG
//デバッグレイヤーをオンに
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableSynchronizedCommandQueueValidation(TRUE);
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

#pragma region アダプタの列挙(P01_02)

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
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion 


#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif

#pragma region コマンドリストをGPUにまとめて命令を送るため (P01_02)

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

#pragma endregion
#pragma region コマンドキューの生成(P01_02)


	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

#pragma endregion 
#pragma region スワップチェーン (P01_02)

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

#pragma endregion 
#pragma region デスクリプタヒープの生成 (P01_02)
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

#pragma endregion
#pragma region バックバッファ(P01_02)

	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);
#pragma endregion 
#pragma region レンダーターゲートビュー(RTV)の生成 (P01_02)

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

#pragma endregion 

#pragma region	深度バッファのリソース設定

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = window_width;								//レンダーターゲットに合わせる
	depthResourceDesc.Height = window_height;							//レンダーターゲットに合わせる
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

#pragma region 深度バッファのリソース設定

	ID3D12Resource* depthBuff = nullptr;
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

#pragma endregion 

#pragma region フェンス (P01_02)

	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion 
#pragma region キー入力 (P02_03)

	//キー入力クラスの作成
	KeyInput* keyInput = new KeyInput();

	keyInput->Initialize(w.hInstance, hwnd);
#pragma endregion

	//DirectX初期化処理 ここまで
#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

#pragma region 定数バッファ用データ構造体を宣言

#pragma endregion
	// 3Dオブジェクトの数
	const size_t kObjectCount = 50;
	// 3Dオブジェクトの配列
	Object3d object3ds[kObjectCount];

#pragma region マテリアル用の定数バッファの初期化(P03_02)

	//定数バッファのGPUリソースのポインタ
	ID3D12Resource* constBuffMaterial = nullptr;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region マテリアル用の定数バッファの転送(P03_02)
	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result));

	float colorR = 1.0f;
	float colorG = 1.0f;
	float colorB = 1.0f;

	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

#pragma endregion
	// 図形の形状変化フラグ
	bool formchange = 0;

#pragma region 三次元用の定数バッファの初期化(P05_02)

	//定数バッファのGPUリソースのポインタ
	ID3D12Resource* constBuffTransform0 = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp2{};				//GPUへの転送用
	cbHeapProp2.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc2{};
	cbResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc2.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc2.Height = 1;
	cbResourceDesc2.DepthOrArraySize = 1;
	cbResourceDesc2.MipLevels = 1;
	cbResourceDesc2.SampleDesc.Count = 1;
	cbResourceDesc2.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&cbHeapProp2,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc2,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform0));
	//assert(SUCCEEDED(result));

#pragma endregion

#pragma region 三次元用の定数バッファの転送(P05_02)

	//定数バッファのマッピング
	//ConstBufferDataTransform* constMapTransform = nullptr;
	//result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);//マッピング
	//assert(SUCCEEDED(result));

	//単位行列を代入
	//constMapTransform0->mat = XMMatrixIdentity();
	////前回の式で計算した行列
	//constMapTransform->mat.r[0].m128_f32[0] = 2.0f / window_width;		//ウインドウ横幅
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f	/ window_height;	//ウインドウ縦幅
	////画面半分の平行移動(05_02_P32)
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;	
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	//平行投影変換
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);


	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);	//視点座標
	XMFLOAT3 target(0, 0, 0);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	float angle = 0.0f;	//カメラの回転角

	//射影変換行列
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//上下画角45度
			(float)window_width / window_height,	//アスペクト比(画面横幅/画面縦幅)
			0.1f, 1000.0f							//前端,奥端
		);

	//スケーリング倍率
	XMFLOAT3 scale;
	//回転角
	XMFLOAT3 rotation;
	//座標
	XMFLOAT3 position;

	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };
	position = { 0.0f,0.0f,0.0f };

	//前回の式で計算した行列
	//XMMATRIX oldVer = XMMatrixIdentity();
	//oldVer.r[0].m128_f32[0] = 2.0f / window_width;		//ウインドウ横幅
	//oldVer.r[1].m128_f32[1] = -2.0f	/ window_height;	//ウインドウ縦幅
	////画面半分の平行移動(05_02_P32)
	//oldVer.r[3].m128_f32[0] = -1.0f;
	//oldVer.r[3].m128_f32[1] = 1.0f;
	////今回の関数で計算した行列
	//XMMATRIX newVer = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

#pragma endregion

#pragma region 二個目のオブジェクト用の定数バッファ

	//定数バッファのGPUリソースのポインタ
	ID3D12Resource* constBuffTransform1 = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	//定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&cbHeapProp2,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc2,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform1));
	//assert(SUCCEEDED(result));

	//定数バッファのマッピング
	//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);//マッピング
	//assert(SUCCEEDED(result));

	//スケーリング倍率
	XMFLOAT3 scale1;
	//回転角
	XMFLOAT3 rotation1;
	//座標
	XMFLOAT3 position1;

	scale1 = { 1.0f,1.0f,1.0f };
	rotation1 = { 0.0f,0.0f,0.0f };
	position1 = { 0.0f,0.0f,0.0f };



#pragma endregion

	//配列内の全オブジェクトに対して
	for (int i = 0; i < _countof(object3ds); i++)
	{
		//初期化
		InitializeObject3d(&object3ds[i], device);

		//親構造のサンプル
		//先頭以外なら
		if (i > 0)
		{
			//親オブジェクトの設定
			object3ds[i].parent = &object3ds[0];
			//親オブジェクトに対する大きさ
			object3ds[i].scale = { 1.0f,1.0f,1.0f };
			//親オブジェクトに対する回転
			object3ds[i].rotation = { 0.0f,0.0f,0.0f };
			//親オブジェクトに対してZ方向-8.0ずらす
			//object3ds[i].position = { 0.0f,0.0f,-8.0f };
		}

		object3ds[1].position = { 10.0f,0.0f,0.0f };
		object3ds[2].position = { 20.0f,0.0f,0.0f };
		object3ds[3].position = { -10.0f,0.0f,0.0f };
		object3ds[4].position = { -20.0f,0.0f,0.0f };
		object3ds[5].position = { 0.0f,10.0f,0.0f };
		object3ds[6].position = { 0.0f,-10.0f,0.0f };
		object3ds[7].position = { 0.0f,-20.0f,0.0f };
		object3ds[8].position = { 10.0f,-30.0f,0.0f };
		object3ds[9].position = { 10.0f,-40.0f,0.0f };
		object3ds[10].position = { -10.0f,-30.0f,0.0f };
		object3ds[11].position = { -10.0f,-40.0f,0.0f };
	}

#pragma	region 頂点データ(P02_01)

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//画像内のxyz座標
	};

	// 頂点データ
	Vertex vertices[] = {
		//	x		y	 z			u	v
		// 前
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// 左下 
		{{ -5.0f, 5.0f, -5.0f }, {}, {0.0f,0.0f}},// 左上 
		{{ 5.0f, -5.0f, -5.0f }, {}, {1.0f,1.0f}},// 右下 
		{{ 5.0f, 5.0f, -5.0f }, {}, {1.0f,0.0f}},// 右上 
		// 後(前面とZ座標の符号が逆)
		{{ -5.0f, -5.0f, 5.0f }, {}, {0.0f,1.0f}},// 左下 
		{{ -5.0f, 5.0f, 5.0f }, {}, {0.0f,0.0f}},// 左上 
		{{ 5.0f, -5.0f, 5.0f }, {}, {1.0f,1.0f}},// 右下 
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f,0.0f}},// 右上 

		// 左
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// 左下 
		{{ -5.0f, 5.0f, -5.0f },{}, {0.0f,0.0f}},// 左上 
		{{ -5.0f, -5.0f, 5.0f },{}, {1.0f,1.0f}},// 右下 
		{{ -5.0f, 5.0f, 5.0f }, {},{1.0f,0.0f}},// 右上 

		// 右(左面とX座標の符号が逆)
		{{ 5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// 左下 
		{{ 5.0f, 5.0f, -5.0f },{}, {0.0f,0.0f}},// 左上 
		{{ 5.0f, -5.0f, 5.0f },{}, {1.0f,1.0f}},// 右下 
		{{ 5.0f, 5.0f, 5.0f }, {},{1.0f,0.0f}},// 右上 

		// 下
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// 左下 
		{{ -5.0f, -5.0f, 5.0f },{}, {0.0f,0.0f}},// 左上 
		{{ 5.0f, -5.0f, -5.0f },{}, {1.0f,1.0f}},// 右下 
		{{ 5.0f, -5.0f, 5.0f },{}, {1.0f,0.0f}},// 右上 
		// 上(下面とY座標の符号が逆)
		{{ -5.0f, 5.0f, -5.0f },{}, {0.0f,1.0f}},// 左下 
		{{ -5.0f, 5.0f, 5.0f },{},{0.0f,0.0f}},// 左上 
		{{ 5.0f, 5.0f, -5.0f },{}, {1.0f,1.0f}},// 右下 
		{{ 5.0f, 5.0f, 5.0f },{}, {1.0f,0.0f}},// 右上 

	};

	//uint16_t indices[] =
	//{
	//	0,1,2, //三角形1つ目
	//	1,2,3, //三角形2つ目
	//};

	unsigned short indices[] =
	{
		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後(前の面に4加算)
		6,5,4,		//三角形3つ目
		6,7,5,		//三角形4つ目

		//左
		10,9,8,		//三角形5つ目
		10,11,9,	//三角形6つ目
		//右
		12,13,14,	//三角形7つ目
		14,13,15,	//三角形8つ目

		//下
		18,17,16,	//三角形5つ目
		18,19,17,	//三角形6つ目
		//上
		20,21,22,	//三角形7つ目
		22,21,23,	//三角形8つ目
	};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

#pragma endregion 

#pragma region 頂点バッファの確保(P02_01)

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

#pragma endregion 

#pragma region 法線の計算

	for (int i = 0; i < _countof(indices) / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

#pragma endregion 

#pragma region 頂点バッファへのデータ転送(P02_01)

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
#pragma endregion

#pragma region インデックスバッファの設定(P03_04)
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//　インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの設定
	ID3D12Resource* indexBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

#pragma endregion

#pragma region インデックスバッファへのデータ転送(P03_04)

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];	//インデックスをコピー
	}

	//マッピング解除
	indexBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region インデックスバッファビューの作成(P03_04)

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region 頂点バッファビューの作成(P02_01)
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};


	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion
#pragma region 頂点シェーダファイルの読み込みとコンパイル(P02_01)

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

#pragma endregion

#pragma region ピクセルシェーダの読み込みとコンパイル(P02_01)

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
#pragma endregion

#pragma region 頂点レイアウト(P02_01)
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			//xyz座標
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //どれぐらいの量を送るか
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			//法線ベクトル(1行で書いたほう見やすい)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		// uv座標
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};
#pragma endregion

#pragma region 画像読み込み
	//画像ファイルの用意
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/dan.jpg",	//「Resources」フォルダの「dan.jpg」
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion

#pragma region 画像読み込み二枚目

	//2枚目用に別の変数を用意しておく
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	// WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata2, scratchImg2);

	ScratchImage mipChain2{};
	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg2.GetImages(), scratchImg2.GetImageCount(), scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain2);
	if (SUCCEEDED(result))
	{
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata2.format = MakeSRGB(metadata2.format);


#pragma endregion

#pragma region 画像読み込み三枚目

	//2枚目用に別の変数を用意しておく
	TexMetadata metadata3{};
	ScratchImage scratchImg3{};
	// WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/tomas.png",
		WIC_FLAGS_NONE,
		&metadata3, scratchImg3);

	ScratchImage mipChain3{};
	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg3.GetImages(), scratchImg3.GetImageCount(), scratchImg3.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain3);
	if (SUCCEEDED(result))
	{
		scratchImg3 = std::move(mipChain3);
		metadata3 = scratchImg3.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata3.format = MakeSRGB(metadata3.format);


#pragma endregion

#pragma region 画像イメージデータの作成(自分で)


	//画像イメージデータの作成

	////横方向ピクセル数
	//const size_t textureWidth = 256;
	////縦方向ピクセル数
	//const size_t textureHeight = 256;
	////配列の要素数
	//const size_t imageDateCount = textureWidth * textureHeight;
	////　画像イメージデータ配列
	//XMFLOAT4* imageDate = new XMFLOAT4[imageDateCount];

	//チャレンジ問題1用の変数
	/*int countNumber = 0;
	int changeColor = 0;*/

	//チャレンジ問題2
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(0, 1);

	////全ピクセルの色を初期化
	//for (size_t i = 0; i < imageDateCount; i++)
	//{
	//	//実行結果
	//	//imageDate[i].x = 1.0f; // R
	//	//imageDate[i].y = 0.0f; // G
	//	//imageDate[i].z = 0.0f; // B
	//	//imageDate[i].w = 1.0f; // A

	//	//練習問題
	//	//imageDate[i].x = 0.0f; // R
	//	//imageDate[i].y = 1.0f; // G
	//	//imageDate[i].z = 0.0f; // B
	//	//imageDate[i].w = 1.0f; // A

	//	//チャレンジ問題1
	//	//if (countNumber < 10)
	//	//{
	//	//	countNumber++;
	//	//}

	//	//if (countNumber >= 10)
	//	//{
	//	//	if (changeColor == 0)
	//	//	{
	//	//		changeColor = 1;
	//	//	}
	//	//	else if (changeColor == 1)
	//	//	{
	//	//		changeColor = 0;
	//	//	}

	//	//	countNumber = 0;
	//	//}

	//	//if (changeColor == 0)
	//	//{
	//	//	imageDate[i].x = 1.0f; // R
	//	//	imageDate[i].y = 0.0f; // G
	//	//	imageDate[i].z = 0.0f; // B
	//	//	imageDate[i].w = 1.0f; // A
	//	//}
	//	//else if(changeColor == 1)
	//	//{
	//	//	imageDate[i].x = 0.0f; // R
	//	//	imageDate[i].y = 0.0f; // G
	//	//	imageDate[i].z = 1.0f; // B
	//	//	imageDate[i].w = 0.0f; // A
	//	//}

	//	//チャレンジ問題2
	//	imageDate[i].x = dist(engine); // R
	//	imageDate[i].y = dist(engine); // G
	//	imageDate[i].z = dist(engine); // B
	//	imageDate[i].w = 1.0f; // A
	//	
	//}

#pragma endregion 

#pragma region テクスチャバッファの設定(P04_02)

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // 幅
	textureResourceDesc.Height = (UINT)metadata.height; // 幅
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region 二枚目のテクスチャバッファの設定(P04_02)

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width; // 幅
	textureResourceDesc2.Height = (UINT)metadata2.height; // 幅
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

#pragma endregion

#pragma region 三枚目のテクスチャバッファの設定(P04_02)

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc3{};
	textureResourceDesc3.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc3.Format = metadata3.format;
	textureResourceDesc3.Width = metadata3.width; // 幅
	textureResourceDesc3.Height = (UINT)metadata3.height; // 幅
	textureResourceDesc3.DepthOrArraySize = (UINT16)metadata3.arraySize;
	textureResourceDesc3.MipLevels = (UINT16)metadata3.mipLevels;
	textureResourceDesc3.SampleDesc.Count = 1;

#pragma endregion



#pragma region テクスチャバッファの生成(P04_02)
	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
#pragma endregion

#pragma region 二枚目のテクスチャバッファの生成(P04_02)
	//テクスチャバッファの生成
	ID3D12Resource* texBuff2 = nullptr;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));
#pragma endregion

#pragma region 三枚目のテクスチャバッファの生成(P04_02)
	//テクスチャバッファの生成
	ID3D12Resource* texBuff3 = nullptr;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff3));
#pragma endregion




#pragma region テクスチャバッファにデータ転送(P04_03_27)

	//全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//二枚目の全ミニマップについて
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img2 = scratchImg2.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,
			img2->pixels,
			(UINT)img2->rowPitch,
			(UINT)img2->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//三枚目の全ミニマップについて
	for (size_t i = 0; i < metadata3.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img3 = scratchImg3.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff3->WriteToSubresource(
			(UINT)i,
			nullptr,
			img3->pixels,
			(UINT)img3->rowPitch,
			(UINT)img3->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,	//全領域へコピー
	//	imageDate,	//元データアドレス
	//	sizeof(XMFLOAT4) * textureWidth,	// 1ラインサイズ
	//	sizeof(XMFLOAT4) * imageDateCount	//　全サイズ
	//);

	//delete[] imageDate;

#pragma endregion

#pragma region デスクリプタヒープ生成(04_02)

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を本にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
#pragma endregion
#pragma region デスクリプタハンドル
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion

#pragma region 深度ビュー用のデスクリプタヒープを作成

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	// デスクリプタヒープの作成
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

#pragma endregion

#pragma region 深度ステンシルビューの生成

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

#pragma endregion

#pragma region シェーダーリソースビュー設定(P04_03)
	//シェーダーリソースビューの設定

	//画像切替用のフラグ
	int changeImage = 1;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
#pragma endregion

#pragma region 二枚目の画像用シェーダーリソースビュー設定(P06_06)

	//デスクリプタのサイズを取得
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	srvHandle.ptr += incrementSize;

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{}; //設定構造体
	srvDesc2.Format = textureResourceDesc2.Format;//RGBA float
	srvDesc2.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle);

#pragma endregion

#pragma region 三枚目の画像用シェーダーリソースビュー設定(P06_06)

	//デスクリプタのサイズを取得
	UINT incrementSize2 = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	srvHandle.ptr += incrementSize2;

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc3{}; //設定構造体
	srvDesc3.Format = textureResourceDesc3.Format;//RGBA float
	srvDesc3.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc3.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc3.Texture2D.MipLevels = textureResourceDesc3.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff3, &srvDesc3, srvHandle);

#pragma endregion




#pragma region グラフィックスパイプライン設定(P02_02_P04)

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
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma region ブレンド設定(03_01)
	// ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画

	//アルファ値の計算式の設定
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う(今から描画しようとしている色)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う(既に描かれている色)

	//RGB値の計算式の設定(加算合成)
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	//減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う

	//色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f - デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;			//使わない

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - ソースのアルファ値

#pragma endregion 

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

#pragma region デスクリプタレンジの設定

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion 
#pragma region ルートパラメータの設定(P03_02)(05_02_P16) 

	//順番は関係ない

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャレンジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える

	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し(タイリング)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し(タイリング)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し(タイリング)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミニマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミニマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ使用可能

#pragma endregion 
#pragma region ルートシグネチャの設定(P02_01_P38)

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

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
#pragma endregion 

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
		keyInput->SaveFrameKey();



#pragma region 頂点バッファへのデータ転送 (P02_01)
		if (keyInput->HasPushedKey(DIK_1))
		{
			/*if (formchange == 0)
			{
				vertices[3] = { +0.5f, +0.5f, 0.0f };
				vertices[4] = { +0.5f, -0.5f, 0.0f };
				vertices[5] = { -0.5f, +0.5f, 0.0f };
				if (keyInput->ReleasedKeyMoment(DIK_1))
				{
					formchange = 1;
				}
			}
			else
			{
				vertices[3] = { -0.5f, -0.5f, 0.0f };
				vertices[4] = { -0.5f, +0.5f, 0.0f };
				vertices[5] = { +0.5f, -0.5f, 0.0f };
				if (keyInput->ReleasedKeyMoment(DIK_1))
				{
					formchange = 0;
				}
			}*/
			// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
			Vertex* vertMap = nullptr;
			result = vertBuff->Map(0, nullptr, (void**)&vertMap);
			assert(SUCCEEDED(result));
			// 全頂点に対して
			for (int i = 0; i < _countof(vertices); i++) {
				vertMap[i] = vertices[i]; // 座標をコピー
			}
			// 繋がりを解除
			vertBuff->Unmap(0, nullptr);
		}
#pragma endregion

#pragma region ターゲットの周りをまわるカメラ(P05_04)
		if (keyInput->HasPushedKey(DIK_D) || keyInput->HasPushedKey(DIK_A))
		{
			if (keyInput->HasPushedKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (keyInput->HasPushedKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			//angleラジアンだけY軸まわりに回転。半径は-100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			//ビュー変換行列を再編成
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

#pragma endregion

#pragma region 図形を連続して動かす

		//いずれかのキーを押していたら
		if (keyInput->HasPushedKey(DIK_UP) || keyInput->HasPushedKey(DIK_DOWN) ||
			keyInput->HasPushedKey(DIK_RIGHT) || keyInput->HasPushedKey(DIK_LEFT))
		{
			//座標を移動する処理(Z座標)
			if (keyInput->HasPushedKey(DIK_UP))
			{
				position.y += 1.0f;
				position1.y += 2.0f;

			}
			else if (keyInput->HasPushedKey(DIK_DOWN))
			{
				position.y -= 1.0f;
				position1.y -= 2.0f;

			}
			if (keyInput->HasPushedKey(DIK_RIGHT))
			{

				position1.x += 2.0f;
			}
			else if (keyInput->HasPushedKey(DIK_LEFT))
			{

				position1.x -= 2.0f;

			}
		}

		if (keyInput->HasPushedKey(DIK_UP) || keyInput->HasPushedKey(DIK_DOWN) ||
			keyInput->HasPushedKey(DIK_RIGHT) || keyInput->HasPushedKey(DIK_LEFT))
		{
			if (keyInput->HasPushedKey(DIK_UP)) { object3ds[0].position.y += 1.0f; }
			else if (keyInput->HasPushedKey(DIK_DOWN)) { object3ds[0].position.y -= 1.0f; }
			if (keyInput->HasPushedKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
			else if (keyInput->HasPushedKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }

			if (keyInput->HasPushedKey(DIK_A)) { object3ds[0].rotation.z += 0.1f; }
			else if (keyInput->HasPushedKey(DIK_D)) { object3ds[0].rotation.z -= 0.1f; }
		}

		if (keyInput->HasPushedKey(DIK_1) || keyInput->PushedKeyMoment(DIK_1))
		{
			changeImage = 1;
		}
		else if (keyInput->HasPushedKey(DIK_2) || keyInput->PushedKeyMoment(DIK_2))
		{
			changeImage = 2;
		}
		else if (keyInput->HasPushedKey(DIK_3) || keyInput->PushedKeyMoment(DIK_3))
		{
			changeImage = 3;
		}


#pragma endregion

#pragma region ワールド変換行列 

		for (size_t i = 0; i < _countof(object3ds); i++)
		{
			UpdateObject3d(&object3ds[i], matView, matProjection);
		}

#pragma region 一つ目のオブジェクトのワールド変換行列

		////ワールド変換行列
		//XMMATRIX matWorld;

		//XMMATRIX matScale;	//スケーリング行列
		//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		//XMMATRIX matRot;	//回転行列
		//matRot = XMMatrixIdentity();
		//matRot *= XMMatrixRotationZ((rotation.z));	//Z軸周りに45度回転
		//matRot *= XMMatrixRotationX((rotation.x));	//X軸周りに15度回転
		//matRot *= XMMatrixRotationY((rotation.y));	//Y軸周りに30度回転

		//XMMATRIX matTrans;	//平行移動行列
		//matTrans = XMMatrixTranslation(position.x, position.y, position.z);	//(-50,0,0)平行移動

		//matWorld = XMMatrixIdentity();	//単位行列を代入して変形をリセット
		//matWorld *= matScale;	//ワールド行列にスケーリングを反映
		//matWorld *= matRot;		//ワールド行列に回転を反映
		//matWorld *= matTrans;	//ワールド行列に平行移動を反映

		////定数バッファにデータ転送
		//constMapTransform0->mat = matWorld * matView * matProjection;

#pragma endregion

#pragma region 二つ目のオブジェクトのワールド変換行列

		////ワールド変換行列
		//XMMATRIX matWorld1;
		//matWorld1 = XMMatrixIdentity();

		////スケーリング行列
		//XMMATRIX matScale1 = XMMatrixScaling(1.0f,1.0f,1.0f);
		//XMMATRIX matRot1 = XMMatrixRotationY(XM_PI/4.0f);
		//XMMATRIX matTrans1 = XMMatrixTranslation(position1.x, position1.y, position1.z);

		////ワールド行列を合成
		//matWorld1 = matScale1 * matRot1 * matTrans1;	
		//
		////定数バッファにデータ転送
		//constMapTransform1->mat = matWorld1 * matView * matProjection;


#pragma endregion

#pragma endregion



#pragma region チャレンジ問題

		//if (keyInput->PushedKeyMoment(DIK_2))
		//{
		//	int WiREFRAMEFlag = 1;
		//	WiREFRAMEFlag--;
		//	if (WiREFRAMEFlag <= 0)
		//	{
		//		if (pipelineDesc.RasterizerState.FillMode == D3D12_FILL_MODE_WIREFRAME)
		//		{
		//			pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		//			WiREFRAMEFlag = 20;
		//			result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
		//			assert(SUCCEEDED(result));
		//		}

		//		else if (pipelineDesc.RasterizerState.FillMode == D3D12_FILL_MODE_SOLID)
		//		{
		//			pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		//			WiREFRAMEFlag = 20;
		//			result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
		//			assert(SUCCEEDED(result));
		//		}
		//	}

		//}

#pragma endregion

#pragma region リソースバリア1 (P01_03)
		// バックバッファの番号を取得(2つなので0番か1番)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex]; // バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 

#pragma region 描画先指定コマンド (P01_03)
		// 2.描画先の変更
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);


		//深度ステンシルビュー用のデスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

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

		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
#pragma region グラフィックコマンド
		// 4.描画コマンドここから

#pragma region ビューポート設定コマンド

		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport;

		//1個目のビューポートを設定 左上
		viewport.Width = window_width;      //横幅
		viewport.Height = window_height;	//縦幅
		viewport.TopLeftX = 0;					//左上X
		viewport.TopLeftY = 0;					//左上Y
		viewport.MinDepth = 0.0f;				//最大深度
		viewport.MaxDepth = 1.0f;				//最小深度

		// ビューポート設定コマンドを、コマンドリストに積む
		commandList->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region シザー矩形の設定コマンド

		// シザー矩形
		D3D12_RECT scissorRect;
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + window_width; // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + window_height; // 切り抜き座標下

		// シザー矩形設定コマンドを、コマンドリストに積む
		commandList->RSSetScissorRects(1, &scissorRect);

#pragma endregion

#pragma region パイプラインステートとルートシグネチャの設定コマンド(P02_01)

		// パイプラインステートとルートシグネチャの設定コマンド
		//今からこのルールで描画を始めますよ
		commandList->SetPipelineState(pipelineState);
		commandList->SetGraphicsRootSignature(rootSignature);
#pragma endregion

#pragma region SRV設定コマンド


		//SRVヒープの設定コマンド
		commandList->SetDescriptorHeaps(1, &srvHeap);
		// SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定

		if (changeImage == 1)
		{
			srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		}
		else if (changeImage == 2)
		{
			srvGpuHandle.ptr += incrementSize;
		}
		else if (changeImage == 3)
		{
			srvGpuHandle.ptr += (incrementSize + incrementSize2);
		}

		commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

#pragma endregion 

#pragma region その他の設定コマンド

		// プリミティブ形状の設定コマンド
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		commandList->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		commandList->IASetIndexBuffer(&ibView);

		//定数バッファビュー(CBV)の設定コマンド
		commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		//全オブジェクトについて処理
		for (int i = 0; i < _countof(object3ds); i++)
		{
			DrawObject3d(&object3ds[i], commandList, vbView, ibView, _countof(indices));
		}

		////0番定数バッファビュー(CBV)の設定コマンド
		//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		//// 描画コマンド
		//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);; // 全ての頂点を使って描画

		////1番定数バッファビュー(CBV)の設定コマンド
		//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		//// 描画コマンド
		//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);; // 全ての頂点を使って描画



#pragma endregion 
#pragma endregion グラフィックコマンド
		// 4.描画コマンドここまで
#pragma region リソースバリア2(P01_03)
		// 5.リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
		commandList->ResourceBarrier(1, &barrierDesc);
#pragma endregion
		// DirectX毎フレーム処理 ここまで
#pragma endregion  DirectX毎フレーム処理

#pragma region 画面入れ替え

#pragma region コマンドのフラッシュ(P01_03)

		// 命令のクローズ
		result = commandList->Close();
		assert(SUCCEEDED(result));
		// コマンドリストの実行
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);
		// 画面に表示するバッファをフリップ(裏表の入替え)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
#pragma endregion 

#pragma region コマンド完了待ち(P01_03)
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
#pragma endregion 

#pragma endregion 画面入れ替え

	}

#pragma region WindowsAPI後始末
	//③ゲーム全体の終了処理
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	delete keyInput;

#pragma endregion WindowsAPI後始末
	return 0;

}

void InitializeObject3d(Object3d* object, ID3D12Device* device)
{
	HRESULT result;

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTransform));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);
	assert(SUCCEEDED(result));
}

void UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動の計算
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);
	matTrans = XMMatrixTranslation(object->position.x, object->position.y, object->position.z);

	object->matworld = XMMatrixIdentity();
	object->matworld *= matScale;
	object->matworld *= matRot;
	object->matworld *= matTrans;

	//親オブジェクトがあれば
	if (object->parent != nullptr)
	{
		//親オブジェクトのワールド行列を掛ける
		object->matworld *= object->parent->matworld;
	}

	//定数バッファへデータ転送
	object->constMapTransform->mat = object->matworld * matview * matProjection;
}

void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices)
{
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}

//定数バッファの生成
//long ConstantBufferResult(long& result, ID3D12Device* device, D3D12_HEAP_PROPERTIES& cbHeapProp,
//	D3D12_RESOURCE_DESC& cbResourceDesc, ID3D12Resource* constBuffTransform)
//{
//
//	assert(device != nullptr && constBuffTransform != nullptr);
//
//	return result = device->CreateCommittedResource(
//		&cbHeapProp,//ヒープ設定
//		D3D12_HEAP_FLAG_NONE,
//		&cbResourceDesc,//リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffTransform));
//	assert(SUCCEEDED(result));
//}

//void CreateConstantBuffer(ID3D12Resource* constBuffMaterial)
//{
//	if(constBuffMaterial == nullptr)
//	//ヒープ設定
//	D3D12_HEAP_PROPERTIES cbHeapProp{};
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//リソース設定
//	D3D12_RESOURCE_DESC cbResourceDesc{};
//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアラインメント
//	cbResourceDesc.Height = 1;
//	cbResourceDesc.DepthOrArraySize = 1;
//	cbResourceDesc.MipLevels = 1;
//	cbResourceDesc.SampleDesc.Count = 1;
//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//}