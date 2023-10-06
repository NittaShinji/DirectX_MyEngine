#include "PostEffect.h"
#include "WindowsAPI.h"
#include "Matrix4.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

//静的メンバ変数の実態
std::string PostEffect::kDefaultTextureDirectoryPath_ = "Resources/";
uint32_t PostEffect::sTextureIndex_;
const size_t kMaxSRVCount = 2056;
std::map<const std::string, uint32_t, std::less<>> PostEffect::textureMap_;
const float PostEffect::clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };	//青っぽい色

PostEffect::PostEffect()
	: Sprite()
{
}

void PostEffect::Initialize(DirectXBasic* directXBasic)
{
	HRESULT result;

	directXBasic_ = directXBasic;

	//頂点バッファ生成
	D3D12_HEAP_PROPERTIES vertexHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC vertResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * kVertexCount_);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->vertBuff_));
	assert(SUCCEEDED(result));

	anchorPoint_ = { 0.0f,0.0f };

	//頂点データ
	vertices[LB] = {
		{ -1.0f , -1.0f, 0.0f }, {0.0f,1.0f}//左下
	};
	vertices[LT] = {
		{ -1.0f ,+1.0f, 0.0f }, {0.0f,0.0f}//左上
	};
	vertices[RB] = {
		{ +1.0f, -1.0f, 0.0f }, {1.0f,1.0f}//右下
	};
	vertices[RT] = {
		{ +1.0f, +1.0f, 0.0f }, {1.0f,0.0f}//右上
	};

	//頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = this->vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if(SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		this->vertBuff_->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	this->vbView_.BufferLocation = this->vertBuff_->GetGPUVirtualAddress();
	this->vbView_.SizeInBytes = sizeof(Vertex) * 4;
	this->vbView_.StrideInBytes = sizeof(Vertex);

	//定数バッファの生成
	D3D12_HEAP_PROPERTIES cbmHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC cbmResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteCommon::ConstBufferDataMaterial) +
			0xff) & ~0xff);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbmHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbmResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffMaterial_));
	assert(SUCCEEDED(result));

	//定数バッファにデータ転送
	SpriteCommon::ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = this->constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);
	if(SUCCEEDED(result))
	{
		constMapMaterial->color = this->color_;
		this->constBuffMaterial_->Unmap(0, nullptr);
	}

	//定数バッファの生成
	D3D12_HEAP_PROPERTIES cbtHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC cbtResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteCommon::ConstBufferDataTransform) +
			0xff) & ~0xff);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbtHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbtResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffTransform_));
	assert(SUCCEEDED(result));

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowsAPI::kWindow_width_,
		(UINT)WindowsAPI::kWindow_height_,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		//テクスチャバッファの生成
		D3D12_HEAP_PROPERTIES textureHeapProp =
			CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		D3D12_CLEAR_VALUE textureClearValue =
			CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

		result = directXBasic_->GetDevice()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&textureClearValue,
			IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(result));

		//テクスチャを赤クリア
		//画素数(1280 x 720 = 921600ピクセル)
		const UINT pixelCount = WindowsAPI::kWindow_width_ * WindowsAPI::kWindow_height_;
		//画像1行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * WindowsAPI::kWindow_width_;
		//画像全体のデータサイズ
		const UINT depthPitch = rowPitch * WindowsAPI::kWindow_height_;
		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for(int j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

		//テクスチャバッファにデータ転送
		result = texBuff[i]->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;

	//SRV用のデスクリプタヒープを生成
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};									//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;						//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		//デスクリプタヒープにSRV生成
		directXBasic_->GetDevice()->CreateShaderResourceView(texBuff[i].Get(),	//ビューと関連付けるバッファ
			&srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i,
				directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}


	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = kRenderTexNum;
	//RTV用デスクリプタヒープを生成
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRBGに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		//デスクリプタヒープにRTV作成
		directXBasic_->GetDevice()->CreateRenderTargetView(texBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
		);
	}

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WindowsAPI::kWindow_width_,
			WindowsAPI::kWindow_height_,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//深度バッファの生成
	D3D12_HEAP_PROPERTIES depthHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_CLEAR_VALUE depthClearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSVを作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;			//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	directXBasic_->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	//パイプライン生成
	CreateGraphicsPipelineState();
}

void PostEffect::Draw()
{
	//定数バッファにデータ転送
	SpriteCommon::ConstBufferDataTransform* constMapTransform = nullptr;
	HRESULT result = this->constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform);
	if(SUCCEEDED(result))
	{
		constMapTransform->mat = MatrixIdentity();

		this->constBuffTransform_->Unmap(0, nullptr);
	}

	//パイプラインステートの設定
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());

	//ルートシグネチャの設定
	//directXBasic_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature_().Get());
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	//プリミティブ形状を設定
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());

	//頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &this->vbView_);
	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* heaps[] = { descHeapSRV.Get() };
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
			directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

	//次のSRVをルートパラメータ3番に設定
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
			directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);


	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, this->constBuffTransform_->GetGPUVirtualAddress());

	if(isInvisible_)
	{
		return;
	}

	//描画コマンド(頂点数、インスタンスの数、最初の頂点のインデックス,データを読み取る前に各インデックスに追加される値)
	directXBasic_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// エラーなら
	if(FAILED(result_))
	{
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
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if(FAILED(result_))
	{
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
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{	//xyz座標

			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //どれぐらいの量を送るか
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},


		{	//uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
	gpipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスクの設定
	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザステート
	gpipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//デプスステンシルステート
	gpipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	 // 常に上書きルール

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//ブレンドステートの設定
	gpipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定(三角形)
	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipelineDesc.NumRenderTargets = 1;								//描画対象は1つ
	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	gpipelineDesc.SampleDesc.Count = 1;								//1ピクセルにつき1回サンプリング

	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);		//t0レジスタ

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);		//t0レジスタ


	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	//スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;


	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	//ルートシグネチャの生成
	result = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	gpipelineDesc.pRootSignature = rootSignature.Get();

	//グラフィックスパイプラインの生成
	result = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

void PostEffect::PreDrawScene()
{
	//リソースバリアを変更(シェーダーリソース描画可能)
	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		D3D12_RESOURCE_BARRIER resBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

		directXBasic_->GetCommandList()->ResourceBarrier(1, &resBarrier);
	}

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[kRenderTexNum];
	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	directXBasic_->GetCommandList()->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	D3D12_VIEWPORT viewPort[kRenderTexNum];
	D3D12_RECT rect[kRenderTexNum];

	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		viewPort[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_);
		rect[i] = CD3DX12_RECT(0, 0, WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_);
	}

	//ビューポートの設定
	directXBasic_->GetCommandList()->RSSetViewports(2, viewPort);
	//シザリング矩形の設定
	directXBasic_->GetCommandList()->RSSetScissorRects(2, rect);

	//全画面クリア
	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		directXBasic_->GetCommandList()->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	directXBasic_->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		//リソースバリアを変更(描画可能→シェーダーリソース)
		D3D12_RESOURCE_BARRIER resBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		directXBasic_->GetCommandList()->ResourceBarrier(1, &resBarrier);
	}

}