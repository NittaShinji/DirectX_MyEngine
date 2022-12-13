#include "Object3d.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;

std::vector<Object3d::Vertex> Object3d::vertices_;
std::vector<unsigned short> Object3d::indices_;


void Object3d::StaticInitialize(Microsoft::WRL::ComPtr<ID3D12Device> device, int window_width, int window_hieght)
{

}

void Object3d::Create()
{

}

void Object3d::InitializeObject3d(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

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
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

void Object3d::Initialize()
{
	//std::vector<Vertex> v = 
	//{
	//	{ -0.5f, -0.5f, 0.0f },{0,0,0}, // 左下 インデックス0
	//	{ -0.5f, +0.5f, 0.0f },{0,0,0}, // 左上 インデックス1
	//	{ +0.5f, -0.5f, 0.0f },{0,0,0}, // 右下 インデックス2
	//	{ +0.5f, +0.5f, 0.0f },{0,0,0}, // 右上 インデックス3
	//}

	//vertices_ =
	//{
	//	{ -0.5f, -0.5f, 0.0f }, // 左下 インデックス0
	//	{ -0.5f, +0.5f, 0.0f }, // 左上 インデックス1
	//	{ +0.5f, -0.5f, 0.0f }, // 右下 インデックス2
	//	{ +0.5f, +0.5f, 0.0f }, // 右上 インデックス3
	//};
	
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	float angle = 0.0f;	//カメラの回転角

	//射影変換行列
	matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//上下画角45度
			(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
			0.1f, 1000.0f							//前端,奥端
		);

	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };
	position = { 0.0f,0.0f,0.0f };

	////配列内の全オブジェクトに対して
	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//	//初期化
	//	InitializeObject3d(&object3ds[i]);

	//	//親構造のサンプル
	//	//先頭以外なら
	//	if (i > 0)
	//	{
	//		//親オブジェクトの設定
	//		object3ds[i].parent = &object3ds[0];
	//		//親オブジェクトに対する大きさ
	//		object3ds[i].scale = { 1.0f,1.0f,1.0f };
	//		//親オブジェクトに対する回転
	//		object3ds[i].rotation = { 0.0f,0.0f,0.0f };
	//		//親オブジェクトに対してZ方向-8.0ずらす
	//		//object3ds[i].position = { 0.0f,0.0f,-8.0f };
	//	}

	//	object3ds[1].position = { 10.0f,0.0f,0.0f };
	//	object3ds[2].position = { 20.0f,0.0f,0.0f };
	//	object3ds[3].position = { -10.0f,0.0f,0.0f };
	//	object3ds[4].position = { -20.0f,0.0f,0.0f };
	//	object3ds[5].position = { 0.0f,10.0f,0.0f };
	//	object3ds[6].position = { 0.0f,-10.0f,0.0f };
	//	object3ds[7].position = { 0.0f,-20.0f,0.0f };
	//	object3ds[8].position = { 10.0f,-30.0f,0.0f };
	//	object3ds[9].position = { 10.0f,-40.0f,0.0f };
	//	object3ds[10].position = { -10.0f,-30.0f,0.0f };
	//	object3ds[11].position = { -10.0f,-40.0f,0.0f };
	//}

}

void Object3d::Update()
{
	//ワールド変換行列
	XMMATRIX matWorld;

	XMMATRIX matScale;	//スケーリング行列
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX matRot;	//回転行列
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ((rotation.z));	//Z軸周りに45度回転
	matRot *= XMMatrixRotationX((rotation.x));	//X軸周りに15度回転
	matRot *= XMMatrixRotationY((rotation.y));	//Y軸周りに30度回転

	XMMATRIX matTrans;	//平行移動行列
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);	//(-50,0,0)平行移動

	matWorld = XMMatrixIdentity();	//単位行列を代入して変形をリセット
	matWorld *= matScale;	//ワールド行列にスケーリングを反映
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映

	//定数バッファにデータ転送
	constMapTransform->mat = matWorld * matView * matProjection;
}

void Object3d::Draw()
{
	// シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + directXBasic_->GetWinWidth(); // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + directXBasic_->GetWinHeight(); // 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	directXBasic_->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	//パイプラインのセット
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	//ルートシグネチャのセット
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブトポロジーのセット
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetIndexBuffer(&ibView);
	////0番定数バッファビュー(CBV)の設定コマンド
	//directXBasic_ ->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	//0番定数バッファビュー(CBV)の設定コマンド
	directXBasic_ ->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// 描画コマンド
	directXBasic_->GetCommandList()->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0); // 全ての頂点を使って描画

	////1番定数バッファビュー(CBV)の設定コマンド
	//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
	//// 描画コマンド
	//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);; // 全ての頂点を使って描画
}

void Object3d::CreateModel()
{
	//ファイルストリーム
	std::ifstream file;
	// .OBJファイルを開く
	file.open("Resources/triangle/triangle.obj");
	//ファイルオープン失敗をチェック
	assert(!file.fail());

	vector<XMFLOAT3>positions;	//頂点座標
	vector<XMFLOAT3>normals;	//法線ベクトル
	vector<XMFLOAT3>texcoords;	//テクスチャUV

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列が"v"なら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			Vertex vertex{};
			vertex.pos = position;
			vertices_.emplace_back(vertex);
		}

		//先頭文字列が"f"ならポリゴン(三角形)
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPosition;
				index_stream >> indexPosition;
				//頂点インデックスに追加
				indices_.emplace_back(indexPosition - 1);
			}
		}

	}

	file.close();

#pragma region 定数バッファ

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

#pragma endregion


	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB =
		static_cast<UINT>(sizeof(Vertex) * vertices_.size());


	//カラーの書き込みと転送
	constMapMaterial->color = XMFLOAT4(0, 1, 0, 0.5f);

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
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
	//Vertex* vertMap = nullptr;
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	std::copy(vertices_.begin(), vertices_.end(), vertMap);

	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

#pragma region 頂点バッファビューの作成

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
	
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//　インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの設定
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result_ = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	std::copy(indices_.begin(), indices_.end(), indexMap);

	//マッピング解除
	indexBuff->Unmap(0, nullptr);

#pragma region インデックスバッファビューの作成

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
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

	// 頂点レイアウト
	inputLayout = 
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1行で書いたほうが見やすい)
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
	
	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = &inputLayout.at(0);
	pipelineDesc.InputLayout.NumElements = inputLayout.size();
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

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

	//RootSignatureSet();

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParam.Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParam.Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える


	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = 1;	//ルートパラメーター数

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();
	
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
	
}

void Object3d::RootSignatureSet()
{
	
}
