#include "Sprite.h"
#include <d3dcompiler.h>
#include <D3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

SpriteCommon* Sprite::spriteCommon_ = nullptr;
DirectXBasic* Sprite::directXBasic_ = nullptr;
KeyInput* Sprite::keys_ = nullptr;
std::string Sprite::kDefaultTextureDirectoryPath_ = "Resources/";

uint32_t Sprite::textureIndex_ = 0;

const size_t kMaxSRVCount = 2056;
std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> Sprite::textureBuffers_;
ID3D12DescriptorHeap* Sprite::srvHeap = nullptr;
UINT64 Sprite::fenceCount;
//D3D12_RESOURCE_DESC Sprite::textureResourceDesc{};

void Sprite::StaticInitialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();
	keys_ = KeyInput::GetInstance();
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を本にSRV用デスクリプタヒープを生成
	HRESULT result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result_));
}

void Sprite::Initialize(uint32_t textureIndex, XMFLOAT2 position, XMFLOAT2 size)
{
	winWide = directXBasic_->GetWinWidth();
	winHeight = directXBasic_->GetWinHeight();
	textureHandleIndex_ = textureIndex;

	scale = { 10.0f,10.0f,10.0f };
	rotation_ = { 0.0f };
	moveSpeed_ = { 0.0f,0.0f };
	anchorPoint_ = { 0.0f,0.0f };

	size_ = size;

	//ウインドウの中心に表示
	//initPosition_ = { float(winWide / 2),float(winHeight / 2) };
	position_ = position;

	////スプライトの座標
	//vertices_.at(LB) = {
	//	{ initPosition_.x , initPosition_.y + size_.y, 0.0f }, {0.0f,1.0f}//左下
	//};
	//vertices_.at(LT) = {
	//	{ initPosition_.x ,initPosition_.y, 0.0f }, {0.0f,0.0f}//左上
	//};
	//vertices_.at(RB) = {
	//	{ initPosition_.x + size_.x, initPosition_.y + size_.y, 0.0f }, {1.0f,1.0f}//右下
	//};
	//vertices_.at(RT) = {
	//	{ initPosition_.x + size_.x, initPosition_.y, 0.0f }, {1.0f,0.0f}//右上
	//};
	
	vertices_.at(LB) = {
		{ 0.0f , size_.y, 0.0f }, {0.0f,1.0f}//左下
	};
	vertices_.at(LT) = {
		{ 0.0f ,0.0f, 0.0f }, {0.0f,0.0f}//左上
	};
	vertices_.at(RB) = {
		{ size_.x, size_.y, 0.0f }, {1.0f,1.0f}//右下
	};
	vertices_.at(RT) = {
		{ size_.x, 0.0f, 0.0f }, {1.0f,0.0f}//右上
	};

	/*vertices_.at(LB).pos.y += initPosition.y;
	vertices_.at(RB).pos.x += initPosition.x;
	vertices_.at(RB).pos.y += initPosition.y;
	vertices_.at(RT).pos.x += initPosition.x;*/

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	//カラーの書き込みと転送
	spriteCommon_->GetConstMapMaterial()->color = XMFLOAT4(0, 1, 0, 0.5f);

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
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

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
}

void Sprite::matUpdate()
{
	//アンカーポイントの設定
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//反転処理　
	//isFlipX_ = true;
	//isFlipY_ = true;

	//左右反転
	if (isFlipX_ == true)
	{
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_ == true)
	{
		top = -top;
		bottom = -bottom;
	}

	//頂点データ
	/*vertices_[LB].pos = { initPosition_.x + left,initPosition_.y + bottom,0.0f };
	vertices_[LT].pos = { initPosition_.x + left,initPosition_.y + top,0.0f };
	vertices_[RB].pos = { initPosition_.x + right,initPosition_.y + bottom,0.0f };
	vertices_[RT].pos = { initPosition_.x + right,initPosition_.y + top,0.0f };*/

	vertices_[LB].pos = { left + position_.x , bottom + position_.y,0.0f };
	vertices_[LT].pos = { left + position_.x, top + position_.y,0.0f };
	vertices_[RB].pos = { right + position_.x,bottom + position_.y,0.0f };
	vertices_[RT].pos = { right + position_.x,top + position_.y,0.0f };

	//いずれかのキーを押していたら
	//座標を移動する処理(Z座標)
	if (keys_->HasPushedKey(DIK_UP)) { moveSpeed_.y -= 0.1f; }
	else if (keys_->HasPushedKey(DIK_DOWN)) { moveSpeed_.y += 0.1f; }
	else
	{
		moveSpeed_.y = 0.0f;
	}
	if (keys_->HasPushedKey(DIK_RIGHT)) { moveSpeed_.x += 0.1f; }
	else if (keys_->HasPushedKey(DIK_LEFT)) { moveSpeed_.x -= 0.1f; }
	else
	{
		moveSpeed_.x = 0.0f;
	}

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	//ワールド変換行列
	XMMATRIX matWorld;

	XMMATRIX matScale;	//スケーリング行列
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	//matScale = XMMatrixScaling(100.0f, 0.5f, 100.0f);

	XMMATRIX matRot;	//回転行列
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ((rotation_));	//Z軸周りに回転

	XMMATRIX matTrans;	//平行移動行列
	matTrans = XMMatrixTranslation(moveSpeed_.x, moveSpeed_.y, 0.0f);	//平行移動
	
	matWorld = XMMatrixIdentity();	//単位行列を代入して変形をリセット
	
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
	//定数バッファにデータ転送
	spriteCommon_->GetConstMapTransform()->mat = matWorld * spriteCommon_->GetConstMapTransform()->mat;
	spriteCommon_->GetConstMapMaterial()->color = color_;
}

void Sprite::Draw()
{
	spriteCommon_->Update();

	//頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//SRVヒープの設定コマンド
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);

	//GPUのSRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	for (uint32_t i = 0; i < textureHandleIndex_; i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, spriteCommon_->GetConstBuffTransform()->GetGPUVirtualAddress());

	if (isInvisible_)
	{
		return;
	}

	//描画コマンド(頂点数、インスタンスの数、最初の頂点のインデックス,データを読み取る前に各インデックスに追加される値)
	directXBasic_->GetCommandList()->DrawInstanced(vertices_.size(), 1, 0, 0);

}
//
//void Sprite::TexMapping()
//{
//
//	//画像イメージデータ配列
//	XMFLOAT4* imageDate;
//	//画像イメージデータ配列
//	imageDate = new XMFLOAT4[imageDateCount];
//
//	//全ピクセルの色を初期化
//	for (size_t i = 0; i < imageDateCount; i++)
//	{
//		imageDate[i].x = 1.0f; // R
//		imageDate[i].y = 0.0f; // G
//		imageDate[i].z = 0.0f; // B
//		imageDate[i].w = 1.0f; // A
//	}
//
//	//ヒープ設定
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty =
//		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//リソース設定
//	D3D12_RESOURCE_DESC textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	textureResourceDesc.Width = textureWidth; // 幅
//	textureResourceDesc.Height = textureWidth; // 幅
//	textureResourceDesc.DepthOrArraySize = 1;
//	textureResourceDesc.MipLevels = 1;
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	//テクスチャバッファの生成
//	//ID3D12Resource* texBuff = nullptr;
//	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff));
//
//	//テクスチャバッファにデータ転送
//	result_ = texBuff->WriteToSubresource(
//		0,
//		nullptr,	//全領域へコピー
//		imageDate,	//元データアドレス
//		sizeof(XMFLOAT4) * textureWidth,	// 1ラインサイズ
//		sizeof(XMFLOAT4) * imageDateCount	//　全サイズ
//	);
//
//	delete[] imageDate;
//}

void Sprite::LoadTexture(const std::string& fileName)
{
	//画像番号
	textureIndex_++;

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
	if (SUCCEEDED(result_))
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
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	
	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	for (uint32_t i= 0; i < textureIndex_; i++)
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
