#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <string.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ComPtr<ID3D12Resource> ParticleManager::texbuff_;
// 頂点バッファ
ComPtr<ID3D12Resource> ParticleManager::vertBuff_;
//頂点データ配列
ParticleManager::Vertex ParticleManager::vertices_[vertexCount];

//SRV用のデスクリプタヒープ
ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap_;
//デスクリプタヒープハンドル
D3D12_CPU_DESCRIPTOR_HANDLE ParticleManager::sSrvHandle_;
// デスクリプタサイズ
UINT ParticleManager::descriptorHandleIncrementSize_;

//テクスチャ番号
uint32_t ParticleManager::sTextureIndex_;

// シェーダリソースビューのハンドル(CPU)
D3D12_CPU_DESCRIPTOR_HANDLE ParticleManager::cpuDescHandleSRV_;
// シェーダリソースビューのハンドル(CPU)
D3D12_GPU_DESCRIPTOR_HANDLE ParticleManager::gpuDescHandleSRV_;

ComPtr<ID3D12PipelineState> ParticleManager::pipelineState_;

ID3D12GraphicsCommandList* ParticleManager::cmdList_;

ID3D12Device* ParticleManager::device_ = nullptr;

ComPtr<ID3D12RootSignature> ParticleManager::rootSignature_;


//定数バッファの生成
template <typename Type1>
ComPtr<ID3D12Resource> ParticleManager::CrateConstBuff(Type1* device)
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
	HRESULT result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	return constBuff_;
}

//XMFLOAT3同士の加算処理
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
};

void ParticleManager::StaticInitialize(ID3D12Device* device)
{
	ParticleManager::device_ = device;
}

void ParticleManager::Initialize()
{
	InitializeGraphicsPipeline();

	LoadTexture("effect1.png");

	CreateModel();

	isMaxParticle_ = false;
}

void ParticleManager::SetScale()
{
	for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		it->scale -= 0.11f;
	}
}

void ParticleManager::Update(Camera* camera)
{
	HRESULT result;

	//寿命が尽きたパーティクルを全削除
	particles_.remove_if(
		[](Particle& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//ビュー変換行列
	matView_ = camera->GetMatView();
	//射影変換行列
	matProjection_ = camera->GetMatProjection();


	//全パーティクル更新
	//for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	//{
	//	//nowParticleCount_++;

	//	//経過フレーム数をカウント
	//	//it->frame++;

	//	////速度に加速度を加算
	//	//it->velocity = it->velocity + it->accel;
	//	////速度による移動
	//	//it->position = it->position + it->velocity;

	//	////進行度を0～1の範囲に換算
	//	//float f = (float)it->frame / it->num_frame;

	//	////スケールの線形補間
	//	//it->scale = (it->e_scale - it->s_scale) * f;
	//	//it->scale += it->s_scale;

	//	//色の移動
	//	/*if(it->color.x < 1)
	//	{
	//		it->color.x = it->color.x + it->colorSpeed.x;
	//	}
	//	else if(it->color.x >= 1)
	//	{
	//		it->color.x = 0;
	//	}*/

	//	/*it->color.x = 1;
	//	it->color.y = 1;
	//	it->color.z = 1;
	//	it->color.w = 1;*/


	//	/*if(nowParticleCount_ >= vertexCount)
	//	{
	//		isMaxParticle_ = true;
	//		break;
	//	}*/
	//}

	//頂点バッファへデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);

	nowParticleCount_ = 0;
	
	if(SUCCEEDED(result))
	{
		for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
		{
			nowParticleCount_++;

			if(nowParticleCount_ >= vertexCount)
			{
				isMaxParticle_ = true;
				break;
			}
			else
			{
				isMaxParticle_ = false;
			}

			//速度に加速度を加算
			it->velocity = it->velocity + it->accel;
			//速度による移動
			it->position = it->position + it->velocity;

			it->frame++;
			//進行度を0～1の範囲に換算
			float f = (float)it->frame / it->num_frame;

			//スケールの線形補間
			it->scale = (it->e_scale - it->s_scale) * f;
			it->scale += it->s_scale;

			//座標
			vertMap->pos.x = it->position.x;
			vertMap->pos.y = it->position.y;
			vertMap->pos.z = it->position.z;

			//スケール
			vertMap->scale = it->scale;

			vertMap->color.x = it->color.x;
			vertMap->color.y = it->color.y;
			vertMap->color.z = it->color.z;
			vertMap->color.w = it->color.w;

			//次の頂点へ
			vertMap++;
		}
		vertBuff_->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;// 行列の合成
	constMap->viewProjection = matView_ * matProjection_;	// 行列の合成
	//constMap->matBillboard = matBillboard;
	constBuff_->Unmap(0, nullptr);
}

void ParticleManager::Draw()
{
	// nullptrチェック
	assert(cmdList_);

	cmdList_->IASetVertexBuffers(0, 1, &vbView_);

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	cmdList_->SetDescriptorHeaps(1, ppHeaps);

	//定数バッファビュー(CBV)をルートパラメータ0番のデスクリプタレンジに設定
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeap_->GetGPUDescriptorHandleForHeapStart();
	//シェーダリソースビューをルートパラメータ1番のデスクリプタレンジに設定
	cmdList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 描画コマンド
	/*for(std::forward_list<Particle>::iterator it = particles_.begin(); it != particles_.end(); it++)
	{
		if(it->frame <= it->num_frame)
		{
			cmdList_->DrawInstanced(_countof(vertices_), 1, 0, 0);
		}
	}*/

	cmdList_->DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()), 1, 0, 0);
}

std::unique_ptr<ParticleManager> ParticleManager::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<ParticleManager> instance = nullptr;
	instance = std::make_unique<ParticleManager>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	//モデル読み込み
	//Model::Load(path);

	instance->Initialize();
	//instance->SetModel(path);

	return instance;
}

void ParticleManager::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, XMFLOAT4 colorSpeed, float start_scale, float end_scale)
{
	//パーティクルのカウントを1増やす
	//nowParticleCount_++;
	//リストに要素を追加
	particles_.emplace_front();
	//追加した要素の参照
	Particle& p = particles_.front();
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.colorSpeed = colorSpeed;
}

void  ParticleManager::InitializeGraphicsPipeline()
{
	//定数バッファの生成
	constBuff_ = CrateConstBuff<ID3D12Device>(device_);

	//定数バッファのマッピング
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMapData_);//マッピング
	assert(SUCCEEDED(result));

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3D10Blob> gsBlob;	//ジオメトリシェーダーオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if(FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if(FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if(FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			//xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //どれぐらいの量を送るか
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			//法線ベクトル
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{
			// uv座標
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc_.GS.pShaderBytecode = gsBlob->GetBufferPointer();
	pipelineDesc_.GS.BytecodeLength = gsBlob->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面をカリング
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// 頂点レイアウトの設定
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	// その他の設定
	pipelineDesc_.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc_.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デプスステンシルステートの設定
	pipelineDesc_.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み禁止
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

		//加算合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			
		blenddesc.DestBlend = D3D12_BLEND_ONE;			

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	}

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//デスクリプタテーブル
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える

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
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// パイプランステートの生成
	result = device_->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));

}

void ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap_));//生成
	if(FAILED(result))
	{
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// コマンドリストをセット
	ParticleManager::cmdList_ = cmdList;

	//パイプラインのセット
	cmdList_->SetPipelineState(pipelineState_.Get());
	//ルートシグネチャのセット
	cmdList_->SetGraphicsRootSignature(rootSignature_.Get());
	//プリミティブトポロジーのセット
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw()
{
	// コマンドリストを解除
	ParticleManager::cmdList_ = nullptr;
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	//std::vector<VertexPosNormalUv> realVertices;
	std::vector<Vertex> realVertices;

	//for (int i = 0; i < vertexCount; i++)
	//{
	//	//X,Y,Z全て{-5.0f, +5.0f}でランダムに分布
	//	const float md_width = 10.0f;
	//	vertices[i].pos.x = (float)rand() / RAND_MAX * md_width - md_width / 2.0f;
	//	vertices[i].pos.y = (float)rand() / RAND_MAX * md_width - md_width / 2.0f;
	//	vertices[i].pos.z = (float)rand() / RAND_MAX * md_width - md_width / 2.0f;
	//}

	UINT sizeVB = static_cast<UINT>(sizeof(vertices_));

	// ヒーププロパティ
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
	result = device_->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));


	// 頂点バッファへのデータ転送
	//VertexPosNormalUv* vertMap = nullptr;
	Vertex* vertMap = nullptr;

	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if(SUCCEEDED(result))
	{
		memcpy(vertMap, vertices_, sizeof(vertices_));
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices_);
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

void ParticleManager::LoadTexture(const std::string& fileName)
{
	const std::string directoryPath = "Resources/";

	//ファイルパスを結合
	std::string filePath = directoryPath + fileName;

	//ユニコード文字列に変換する
	wchar_t wfilePath[128];
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wfilePath, _countof(wfilePath));

	//画像ファイルの用意
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	HRESULT result = LoadFromWICFile(
		wfilePath, WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if(SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//CD3DX12_HEAP_PROPERTIES textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // 幅
	textureResourceDesc.Height = (UINT)metadata.height; // 幅
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		//IID_PPV_ARGS(&textureBuffers_[0]));
		IID_PPV_ARGS(&texbuff_));

	textureHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	//全ミニマップについて
	for(size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//テクスチャバッファにデータ転送

		result = texbuff_->WriteToSubresource(

			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result));
	}

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount_;

	//設定を本にSRV用デスクリプタヒープを生成
	result = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&descHeap_));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	sSrvHandle_ = descHeap_->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device_->CreateShaderResourceView(texbuff_.Get(), &srvDesc, sSrvHandle_);

}


