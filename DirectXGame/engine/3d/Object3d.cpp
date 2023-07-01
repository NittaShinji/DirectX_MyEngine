#include "Object3d.h"
#include <d3dcompiler.h>
#include "MathUtillity.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

std::string Object3d::kDefaultTextureDirectoryPath_ = "Resources/";
DirectXBasic* Object3d::directXBasic_ = nullptr;
KeyInput* Object3d::keys_ = nullptr;
LightGroup* Object3d::lightGroup_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Object3d::pipelineState_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> Object3d::rootSignature_ = nullptr;

//定数バッファの生成
template <typename Type1>
ComPtr<ID3D12Resource> Object3d::CrateConstBuff(Type1* directXBasic_)
{
	ComPtr<ID3D12Resource> constBuff_;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(Type1) + 0xff) & ~0xff;	//256バイトアラインメント
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
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	return constBuff_;
}

void Object3d::CrateConstBuffandMapping()
{
}

void Object3d::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
}

void Object3d::Initialize(const std::string& path, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& Modelscale)
{
	/*scale_ = Modelscale;
	rotation_ = rotation;
	transform_ = position;*/

	//定数バッファの生成
	constBuffTransform_ = CrateConstBuff<DirectXBasic>(directXBasic_);
	constBuffMaterial_ = CrateConstBuff<DirectXBasic>(directXBasic_);
	constBuffLight_ = CrateConstBuff<DirectXBasic>(directXBasic_);

	//定数バッファのマッピング
	HRESULT result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);//マッピング
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBuffLight_->Map(0, nullptr, (void**)&constMapLight_);//マッピング
	assert(SUCCEEDED(result));

	constMapLight_->lightv = { 0,0,1 };
	constMapLight_->lightcolor = { 1,1,1 };

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
		&vsBlob_, &errorBlob_);
	// エラーなら
	if(FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
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
		&psBlob_, &errorBlob_);

	// エラーなら
	if(FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

	//グラフィックスパイプラインの設定
	// シェーダーの設定
	pipelineDesc_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob_->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリング
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma region ブレンド設定

	// 頂点レイアウトの設定
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc_.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc_.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デプスステンシルステートの設定
	pipelineDesc_.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければ合格
	pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

#pragma region ブレンド設定
	// ブレンドステート



	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		//レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[i];
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
	}

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[4] = {};

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

	//定数バッファ1番
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える


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
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// パイプランステートの生成
	result = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));

	model_.SetName(path);
	model_.SetInfomation(*Model::GetMODELVALUE(path));
}

Object3d::~Object3d()
{
	delete sprite_;
}

Object3d::Object3d()
{

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

	cameraPos_ = camera_->GetEye();


	XMFLOAT3 move = { 0,0,0 };

	//いずれかのキーを押していたら
	//座標を移動する処理
	if(keys_->HasPushedKey(DIK_RIGHT)) { rotation_.x += 0.4f; }
	else if(keys_->HasPushedKey(DIK_LEFT)) { rotation_.x -= 0.4f; }
	else {}
	if(keys_->HasPushedKey(DIK_P)) { rotation_.y += 0.4f; }
	else if(keys_->HasPushedKey(DIK_L)) { rotation_.y -= 0.4f; }
	else {}

	//スケール、回転、平行移動の計算
	matScale_ = XMMatrixIdentity();
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot_ = XMMatrixIdentity();

	//XMFLOAT3 radian = {0.0f,0.0f,0.0f};
	//radian.x = MathUtillty::Toradian(XMConvertToRadians(rotation_.x));
	//radian.y = MathUtillty::Toradian(XMConvertToRadians(rotation_.y));
	//radian.z = MathUtillty::Toradian(XMConvertToRadians(rotation_.z));

	matRot_ *= XMMatrixRotationZ(rotation_.z);
	matRot_ *= XMMatrixRotationX(rotation_.x);
	matRot_ *= XMMatrixRotationY(rotation_.y);

	/*matRot_ *= XMMatrixRotationZ(XMConvertToRadians((rotation_.z)));
	matRot_ *= XMMatrixRotationX(XMConvertToRadians((rotation_.x)));
	matRot_ *= XMMatrixRotationY(XMConvertToRadians((rotation_.y)));*/

	matTrans_ = XMMatrixIdentity();
	matTrans_ = XMMatrixTranslation(transform_.x, transform_.y, transform_.z);

	matWorld_ = XMMatrixIdentity();
	matWorld_ *= matScale_;
	matWorld_ *= matRot_;
	matWorld_ *= matTrans_;

	model_.Update();

	/*if(colorFlag_ == true)
	{
		constMapTransform->color = { 1,0,0,1 };
	}
	else if(colorFlag_ == false)
	{
		constMapTransform->color = { 1,1,1,1 };
	}*/

	//定数バッファへデータ転送
	constMapTransform_->worldMatrix = matWorld_;
	constMapTransform_->viewProjection = (matView_ * matProjection_);
	constMapTransform_->cameraPos = cameraPos_;

	//定数バッファのマッピング
	HRESULT result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);//マッピング
	constBuffMaterial_->SetName(L"constBuffMaterial");
	assert(SUCCEEDED(result));

	constMapMaterial_->ambient = model_.GetInfomation()->material.ambient;
	constMapMaterial_->diffuse = model_.GetInfomation()->material.diffuse;
	constMapMaterial_->specular = model_.GetInfomation()->material.specular;
	constMapMaterial_->alpha = model_.GetInfomation()->material.alpha;

	constBuffMaterial_->Unmap(0, nullptr);
}

void Object3d::Draw()
{
	//頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &model_.GetInfomation()->vbView);
	//インデックスバッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetIndexBuffer(&model_.GetInfomation()->ibView);

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { model_.GetInfomation()->srvHeap.Get() };
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, ppHeaps);

	//GPUのSRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = model_.GetInfomation()->srvHeap->GetGPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for(uint32_t i = 0; i < model_.GetTexIndex(); i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	// SRVヒープの先頭にあるSRVをルートパラメータ2番のデスクリプタレンジに設定
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuffLight_->GetGPUVirtualAddress());

	//ライトの描画
	lightGroup_->Draw(directXBasic_->GetCommandList().Get(), 3);

	//描画コマンド
	directXBasic_->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(model_.GetInfomation()->indices.size()), 1, 0, 0, 0);
}

void Object3d::BeforeDraw()
{
	//パイプラインのセット
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	//ルートシグネチャのセット
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	//プリミティブトポロジーのセット
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::AfterDraw()
{

}

XMFLOAT3 Object3d::GetWorldPos() const
{
	//全ての壁の座標を渡す
	XMFLOAT3 resutVec = { 0,0,0 };

	//ワールド行列の平行移動成分を取得(ワールド座標)
	resutVec.x = matWorld_.r[3].m128_f32[0];
	resutVec.y = matWorld_.r[3].m128_f32[1];
	resutVec.z = matWorld_.r[3].m128_f32[2];

	return resutVec;
}

void Object3d::Create(Model* model)
{

}