#include "Object3d.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

std::string Object3d::kDefaultTextureDirectoryPath_ = "Resources/";
DirectXBasic* Object3d::directXBasic_ = nullptr;
KeyInput* Object3d::keys_ = nullptr;

void Object3d::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
}

Object3d::~Object3d()
{
	delete sprite_;
}

Object3d::Object3d(const std::string& path, XMFLOAT3 position)
//Object3d::Object3d(DirectXBasic* directXBasic,XMFLOAT3 position)
{
	/*model_ = model;*/
	//model_.Load(path);

	scale = { 20.0f,20.0f,20.0f };
	rotation = { 0.0f,0.0f,0.0f };
	transform = position;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDateTransform) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	HRESULT result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//マッピング
	assert(SUCCEEDED(result));

	//単位行列を代入
	constMapTransform->mat = XMMatrixIdentity();

	//関数が作れるまでの応急処置
	{
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
		HRESULT result = directXBasic_->GetDevice()->CreateCommittedResource(
			&cbHeapProp,//ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,//リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
		assert(SUCCEEDED(result));

		
		//constMapColor->color = { 1,1,0,1 };

	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC  inputLayout[] =
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


#pragma region 頂点シェーダの読み込みとコンパイル(P02_01)

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Object3DVS.hlsl", // シェーダファイル名
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

#pragma region ピクセルシェーダーの読み込み
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Object3DPS.hlsl", // シェーダファイル名
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

	//グラフィックスパイプラインの設定
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma region ブレンド設定

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
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

#pragma region ブレンド設定
	// ブレンドステート
	
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

//#pragma endregion

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

	//定数バッファ1番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[1].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[1].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

	//テクスチャレジスタ0番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//デスクリプタテーブル
	rootParams[2].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[2].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える

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

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	result = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	model_.SetName(path);
	model_.SetInfomation(*Model::GetMODELVALUE(path));
}

void Object3d::SetModel(const std::string& path)
{
	//検索キーからモデル情報を検索し、モデルを割り当てる
	model_.SetName(path);
	model_.SetInfomation(*Model::GetMODELVALUE(path));
}

void Object3d::Update(Camera* camera)
{
	//使用するカメラをセット
	camera_ = camera;

	//ビュー変換行列
	matView_ = camera_->GetMatView();
	//射影変換行列
	matProjection_ = camera_->GetMatProjection();

	//XMMATRIX matView;
	//XMFLOAT3 eye(0, 0, -100);	//視点座標
	//XMFLOAT3 target(0, 0, 0);	//注視点座標
	//XMFLOAT3 up(0, 1, 0);		//上方向ベクトル
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//float angle = 0.0f;	//カメラの回転角

	//射影変換行列
	//XMMATRIX matProjection =
	//	XMMatrixPerspectiveFovLH(
	//		XMConvertToRadians(45.0f),				//上下画角45度
	//		(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
	//		0.1f, 1000.0f							//前端,奥端
	//	);

	
	XMFLOAT3 move = { 0,0,0 };

	//いずれかのキーを押していたら
	//座標を移動する処理
	if (keys_->HasPushedKey(DIK_UP)) { transform.z += 0.4f; }
	else if (keys_->HasPushedKey(DIK_DOWN)) { transform.z -= 0.4f; }
	else{}
	if (keys_->HasPushedKey(DIK_RIGHT)) { transform.x += 0.4f; }
	else if (keys_->HasPushedKey(DIK_LEFT)) { transform.x -= 0.4f; }
	else{}
	if (keys_->HasPushedKey(DIK_P)) { transform.y += 0.4f; }
	else if (keys_->HasPushedKey(DIK_L)) { transform.y -= 0.4f; }
	else{}

	XMMATRIX matScale, matRot, matTrans;

	//ワールド変換行列
	XMMATRIX matWorld;
	
	matWorld = XMMatrixIdentity();

	//スケール、回転、平行移動の計算
	matScale = XMMatrixIdentity();
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixIdentity();
	matTrans = XMMatrixTranslation(transform.x, transform.y, transform.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	model_.Update();
	
	//定数バッファへデータ転送
	constMapTransform->mat = matWorld * matView_ * matProjection_;

	//定数バッファのマッピング
	HRESULT result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	constBuffMaterial->SetName(L"constBuffMaterial");
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = model_.GetInfomation()->material_.ambient;
	constMapMaterial->diffuse = model_.GetInfomation()->material_.diffuse;
	constMapMaterial->specular = model_.GetInfomation()->material_.specular;
	constMapMaterial->alpha = model_.GetInfomation()->material_.alpha;

	constBuffMaterial->Unmap(0, nullptr);

}

void Object3d::Draw()
{
	//頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &model_.GetInfomation()->vbView);
	//インデックスバッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetIndexBuffer(&model_.GetInfomation()->ibView);
	
	//SRVヒープの設定コマンド
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, &model_.GetInfomation()->srvHeap);
	
	//デスクリプタヒープの配列をセットするコマンド
	/*ID3D12DescriptorHeap* ppHeaps[] = { model_.GetInfomation()->srvHeap };
	directXBasic_->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);*/

	////デスクリプタヒープの配列をセットするコマンド
	//ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
	//directXBasic_->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	 
	//GPUのSRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = model_.GetInfomation()->srvHeap->GetGPUDescriptorHandleForHeapStart();
	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32_t i = 0; i < model_.GetTexIndex(); i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	// SRVヒープの先頭にあるSRVをルートパラメータ2番のデスクリプタレンジに設定
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffMaterial->GetGPUVirtualAddress());

	//描画コマンド
	directXBasic_->GetCommandList()->DrawIndexedInstanced(model_.GetInfomation()->indices_.size(), 1, 0, 0, 0);
}

void Object3d::LoadTexture(uint32_t textureIndex, const std::string& fileName)
{
}

void Object3d::BeforeDraw()
{
	//パイプラインのセット
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	//ルートシグネチャのセット
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブトポロジーのセット
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::AfterDraw()
{

}

