#include "SpriteCommon.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

std::string SpriteCommon::kDefaultTextureDirectoryPath_ = "Resources/";
const size_t kMaxSRVCount = 2056;

std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> SpriteCommon::textureBuffers_;

template <typename Type1, typename Type2, typename Type3>
void SpriteCommon::CrateConstBuff(Type1*& constBuffer, Type2*& constMapData, Type3* directXBasic_)
//Comptrなら&で参照
//*&で参照
{
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(Type2) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer));
	assert(SUCCEEDED(result_));


	//定数バッファのマッピング
	result_ = constBuffer->Map(0, nullptr, (void**)&constMapData);//マッピング
	assert(SUCCEEDED(result_));

}

void SpriteCommon::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	//ID3D12Resource* constBuffMaterial_ = constBuffMaterial.Get();

	////SpriteCommon::CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial.Get(), constMapMaterial, directXBasic_);
	//CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial_, constMapMaterial, directXBasic_);

	ID3D12Resource* constBuffMaterial_ = constBuffMaterial.Get();
	ID3D12Resource* constBuffTransform_ = constBuffTransform.Get();

	CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial_, constMapMaterial, directXBasic_);
	CrateConstBuff<ID3D12Resource, ConstBufferDataTransform, DirectXBasic>(constBuffTransform_, constMapTransform, directXBasic_);

	//CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial_.GetAddressOf(), constMapMaterial, directXBasic_);
	//CrateConstBuff<ID3D12Resource, ConstBufferDataTransform, DirectXBasic>(constBuffTransform_.GetAddressOf(), constMapTransform, directXBasic_);

	constBuffMaterial = constBuffMaterial_;
	constBuffTransform = constBuffTransform_;

	//単位行列を代入
	constMapTransform->mat = XMMatrixIdentity();
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXBasic_->GetWinWidth();		//ウインドウ横幅
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXBasic_->GetWinHeight();	//ウインドウ縦幅
	//画面半分の平行移動
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を本にSRV用デスクリプタヒープを生成
	HRESULT result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result_));
}


void SpriteCommon::CrateConstBuffTransform()
{
	//CrateConstBuff<ID3D12Resource*, ConstBufferDataTransform*, DirectXBasic*>(constBuffTransform.Get(), constMapTransform, directXBasic_);
	//
	////単位行列を代入
	//constMapTransform->mat = XMMatrixIdentity();
	//constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXBasic_->GetWinWidth();		//ウインドウ横幅
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXBasic_->GetWinHeight();	//ウインドウ縦幅
	////画面半分の平行移動
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
}

//半透明合成の初期化
//void SpriteCommon::Initialize(DirectXBasic* directXBasic)
//{
//	directXBasic_ = directXBasic;
//
//	//ヒープ設定
//	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//リソース設定
//	D3D12_RESOURCE_DESC cbResourceDesc{};
//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
//	cbResourceDesc.Height = 1;
//	cbResourceDesc.DepthOrArraySize = 1;
//	cbResourceDesc.MipLevels = 1;
//	cbResourceDesc.SampleDesc.Count = 1;
//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//定数バッファの生成
//	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
//		&cbHeapProp,//ヒープ設定
//		D3D12_HEAP_FLAG_NONE,
//		&cbResourceDesc,//リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffMaterial));
//	assert(SUCCEEDED(result_));
//
//	//定数バッファのマッピング
//	result_ = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
//	assert(SUCCEEDED(result_));
//
//
//	//関数が作れるまでの応急処置
//	{
//		//ヒープ設定
//		D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
//		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//		//リソース設定
//		D3D12_RESOURCE_DESC cbResourceDesc{};
//		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
//		cbResourceDesc.Height = 1;
//		cbResourceDesc.DepthOrArraySize = 1;
//		cbResourceDesc.MipLevels = 1;
//		cbResourceDesc.SampleDesc.Count = 1;
//		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//		//定数バッファの生成
//		result_ = directXBasic_->GetDevice()->CreateCommittedResource(
//			&cbHeapProp,//ヒープ設定
//			D3D12_HEAP_FLAG_NONE,
//			&cbResourceDesc,//リソース設定
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&constBuffTransform));
//		assert(SUCCEEDED(result_));
//
//		//定数バッファのマッピング
//		result_ = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//マッピング
//		assert(SUCCEEDED(result_));
//
//		//単位行列を代入
//		constMapTransform->mat = XMMatrixIdentity();
//		constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXBasic_->GetWinWidth();		//ウインドウ横幅
//		constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXBasic_->GetWinHeight();	//ウインドウ縦幅
//		//画面半分の平行移動
//		constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
//		constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
//
//	}
//
//	//デスクリプタヒープの設定
//	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
//	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
//	srvHeapDesc.NumDescriptors = kMaxSRVCount;
//
//	//設定を本にSRV用デスクリプタヒープを生成
//	HRESULT result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
//	assert(SUCCEEDED(result_));
//}



void SpriteCommon::ShaderLoad()
{

#pragma region 頂点シェーダの読み込みとコンパイル(P02_01)

	// 頂点シェーダの読み込みとコンパイル
	HRESULT result_ = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", // シェーダファイル名
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
#pragma endregion

#pragma region ピクセルシェーダーの読み込み
	// ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl", // シェーダファイル名
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

#pragma endregion

}

//半透明
void SpriteCommon::SemiTransparent()
{
	//頂点レイアウトの設定
	VertexLayoutSet();
	//グラフィックスパイプライン設定
	PipelineSet();

#pragma region ブレンド設定
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

	RootSignatureSet();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

}

//加算合成
void SpriteCommon::Add()
{
	//頂点レイアウトの設定
	VertexLayoutSet();

	//グラフィックスパイプライン設定
	PipelineSet();

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

	RootSignatureSet();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

}

//減算合成
void SpriteCommon::Sub()
{
	//頂点レイアウトの設定
	VertexLayoutSet();

	//グラフィックスパイプライン設定
	PipelineSet();

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

	RootSignatureSet();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
}

//反転
void SpriteCommon::InvertColor()
{
	//頂点レイアウトの設定
	VertexLayoutSet();

	//グラフィックスパイプライン設定
	PipelineSet();

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

	RootSignatureSet();

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
}

//画像読み込み
void SpriteCommon::LoadTexture(const std::string& fileName)
{

	//画像番号
	textureIndex_++;

	//画像の文字列と画像番号を格納
	textureMap.emplace(fileName, textureIndex_);

	//ディレクトリパスとファイル名を連結しを得る
	std::string fullPath = kDefaultTextureDirectoryPath_ + fileName;

	//ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

	//ワイド文字列に変換
	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

	//画像ファイルの用意
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	HRESULT result_ = LoadFromWICFile(
		wfilePath.data(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//ミニマップ生成
	result_ = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if(SUCCEEDED(result_))
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
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffers_[textureIndex_]));

	/*SetWidth(textureResourceDesc.Width);
	SetHeight(textureResourceDesc.Height);*/

	//全ミニマップについて
	for(size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//テクスチャバッファにデータ転送

		result_ = textureBuffers_[textureIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result_));
	}


	//----------------------------------

	////ヒープ設定
	//D3D12_HEAP_PROPERTIES textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	////リソース設定
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = metadata.format;
	//textureResourceDesc.Width = metadata.width; // 幅
	//textureResourceDesc.Height = (UINT)metadata.height; // 幅
	//textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	//textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	//textureResourceDesc.SampleDesc.Count = 1;

	////テクスチャバッファの生成
	//result_ = directXBasic_->GetDevice()->CreateCommittedResource(
	//	&textureHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&textureResourceDesc,
	//	D3D12_RESOURCE_STATE_COPY_DEST,
	//	nullptr,
	//	IID_PPV_ARGS(&textureBuffers_[textureIndex_]));

	////レイアウト取得
	//D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint;
	//UINT64  total_bytes = 0;
	//directXBasic_->GetDevice()->GetCopyableFootprints(&textureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	////転送用のバッファを作成
	////転送用ヒープ設定
	//D3D12_HEAP_PROPERTIES  uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	////転送用リソース設定
	//D3D12_RESOURCE_DESC  uploadDesc;
	//memset(&uploadDesc, 0, sizeof(uploadDesc));
	//uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//uploadDesc.Width = total_bytes; // 幅
	//uploadDesc.Height = 1; // 幅
	//uploadDesc.DepthOrArraySize = 1;
	//uploadDesc.MipLevels = 1;
	//uploadDesc.SampleDesc.Count = 1;
	//uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	////転送用テクスチャバッファの生成
	//ID3D12Resource* uploadBuffer = nullptr;
	//result_ = directXBasic_->GetDevice()->CreateCommittedResource(&uploadHeap,
	//	D3D12_HEAP_FLAG_NONE, &uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));

	////転送用バッファへの書き込み
	///*void* ptr = nullptr;
	//uploadBuffer->Map(0, nullptr, &ptr);*/
	////for(size_t i = 0; i < metadata.mipLevels; i++)
	////{
	////	//ミニマップレベルを指定してイメージを取得
	////	//const Image* img = scratchImg.GetImage(i, 0, 0);
	////	/*const uint8_t* pixel[] = { 0 };
	////	pixel[i] = scratchImg.GetPixels();*/
	////	const uint8_t* pixel = scratchImg.GetImage(i,0,0)->pixels;

	////	/*uint8_t pixelData[];
	////	size_t pixelDataSize = scratchImg.GetPixelsSize();*/

	////	//memcpy(reinterpret_cast<unsigned char*>(ptr) + footprint.Offset, img->pixels, scratchImg.GetPixelsSize());
	////	
	////}
	////全ミニマップについて
	////for(size_t i = 0; i < metadata.mipLevels; i++)
	////{
	////	//ミニマップレベルを指定してイメージを取得
	////	const Image* img = scratchImg.GetImage(i, 0, 0);

	////	result_ = uploadBuffer->Map(
	////				(UINT)i,
	////				nullptr,
	////				&ptr);

	////	memcpy(reinterpret_cast<unsigned char*>(ptr) + footprint.Offset, img->pixels, sizeof(img->pixels));

	////	//textureBuffers_[textureIndex_]->
	////}

	////全ミニマップについて
	//for(size_t i = 0; i < metadata.mipLevels; i++)
	//{
	//	//ミニマップレベルを指定してイメージを取得
	//	const Image* img = scratchImg.GetImage(i, 0, 0);

	//	//テクスチャバッファにデータ転送

	//	result_ = uploadBuffer->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,
	//		img->pixels,
	//		(UINT)img->rowPitch,
	//		(UINT)img->slicePitch
	//	);

	//	assert(SUCCEEDED(result_));
	//}

	//
	////コピーコマンド作成
	//D3D12_TEXTURE_COPY_LOCATION  dest;
	//memset(&dest, 0, sizeof(dest));
	//dest.pResource = textureBuffers_[textureIndex_].Get();
	//dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	//dest.SubresourceIndex = 0;

	//D3D12_TEXTURE_COPY_LOCATION  src;
	//memset(&src, 0, sizeof(src));
	//src.pResource = uploadBuffer;
	//src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	//src.PlacedFootprint = footprint;

	////転送コマンドを格納
	//ID3D12GraphicsCommandList* iCommandList = directXBasic_->GetCommandList().Get();
	//iCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

	////コマンドキューとコマンドリストが描画用と共用なのでリソースバリアが必要
	////リソースバリアの設定
	//D3D12_RESOURCE_BARRIER  barrier;
	//memset(&barrier, 0, sizeof(barrier));
	//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//barrier.Transition.pResource = textureBuffers_[textureIndex_].Get();
	//barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	////リソースバリアの生成
	//iCommandList->ResourceBarrier(1, &barrier);

	//----------------------------

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	for(uint32_t i = 0; i < textureIndex_; i++)
	{
		srvHandle.ptr += incrementSize;
	}

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[textureIndex_].Get(), &srvDesc, srvHandle);
}

void SpriteCommon::Update()
{
	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
}

void SpriteCommon::VertexLayoutSet()
{
	// 頂点レイアウト
	inputLayout =
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

		// uv座標
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//座標以外に、色、テクスチャUVなどを渡す場合はさらに続ける
	};
}

void SpriteCommon::PipelineSet()
{
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

#pragma region ブレンド設定(03_01)

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = &inputLayout.at(0);
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout.size());

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

}

void SpriteCommon::RootSignatureSet()
{
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
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//デスクリプタテーブル
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

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメーター数
	//サンプラーの設定をルートシグネチャに追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	HRESULT result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
}

void SpriteCommon::DescriptorHeapSet()
{

	////SRVヒープの設定コマンド
	//directXBasic_->GetCommandList()->SetDescriptorHeaps(1, &srvHeap_);

	////GPUのSRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	//srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

}

void SpriteCommon::BeforeDraw()
{
	//パイプラインのセット
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	//ルートシグネチャのセット
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブトポロジーのセット
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//デスクリプタヒープのセット
	DescriptorHeapSet();
}

void SpriteCommon::AfterDraw() {}

