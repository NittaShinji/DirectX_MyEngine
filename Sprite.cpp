#include "Sprite.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();

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

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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
	result_ = directXBasic_->GetResult();
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

#pragma region 頂点バッファビューの作成(P02_01)
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	//vbView.StrideInBytes = sizeof(vertices[0]);
	vbView.StrideInBytes = sizeof(XMFLOAT3);


	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
#pragma endregion

#pragma region 頂点シェーダファイルの読み込みとコンパイル(P02_01)
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
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	//レンダーターゲットのブレンド設定
	//D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	//blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画

	//アルファ値の計算式の設定
	//blenddesc.BlendEnable = true;					//ブレンドを有効にする
	//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	//blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う(今から描画しようとしている色)
	//blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う(既に描かれている色)

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
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - ソースのアルファ値

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

	// ルートシグネチャ
	//ComPtr<ID3D12RootSignature> rootSignature;
	ID3D12RootSignature* rootSignature;

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
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	//ComPtr<ID3D12PipelineState> pipelineState;
	ID3D12PipelineState *pipelineState;


	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

	directXBasic_->GetCommandList()->SetPipelineState(pipelineState);
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature);

	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//描画コマンド
	directXBasic_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}


