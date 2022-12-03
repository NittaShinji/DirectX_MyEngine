#include "SpriteCommon.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

//半透明合成の初期化
void SpriteCommon::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

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
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result_));

	//定数バッファのマッピング
	
	result_ = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result_));


	//値を書き込むと自動的に転送される
	//constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);	//RGBAで半透明の赤

#pragma region 頂点シェーダの読み込みとコンパイル(P02_01)
	
	
	// 頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// エラーなら
	if (FAILED(result_)) {
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

#pragma region ピクセルシェーダーの読み込み
	// ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result_)) {
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

}

void SpriteCommon::SemiTransparent()
{

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

		//{
		//	//法線ベクトル(1行で書いたほう見やすい)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv座標
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};
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
	
	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParam.Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える


	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	rootSignatureDesc.pParameters = &rootParam;						//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = 1;			//ルートパラメーター数
	 
	
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::Add()
{

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

		//{
		//	//法線ベクトル(1行で書いたほう見やすい)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv座標
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};
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
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

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

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// パイプラインにルートシグネチャをセット
	//pipelineDesc.pRootSignature = rootSignature.Get();
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::Sub()
{

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

		//{
		//	//法線ベクトル(1行で書いたほう見やすい)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv座標
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};
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

	//減算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う

	
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

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::InvertColor()
{

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

		//{
		//	//法線ベクトル(1行で書いたほう見やすい)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv座標
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};
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

	//色反転
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f - デストカラーの値
	blenddesc.DestBlend = D3D12_BLEND_ZERO;			//使わない

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

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::Update()
{
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}