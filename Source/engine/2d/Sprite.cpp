#include "Sprite.h"
#include "WindowsAPI.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma warning(push)
#pragma warning(disable:4061)
#pragma warning(disable:4820)
#include <D3dx12.h>
#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace MathUtillty;

SpriteCommon* Sprite::spriteCommon_ = nullptr;
TextureManager* Sprite::textureManager_ = nullptr;
DirectXBasic* Sprite::directXBasic_ = nullptr;
KeyInput* Sprite::keys_ = nullptr;
Matrix4 Sprite::matProjection_;

template<typename Type1, typename Type2>
ComPtr<ID3D12Resource> Sprite::CrateConstBuff(Type1*& constMapData, Type2* directXBasic_)
{
	ComPtr<ID3D12Resource> constBuff_;
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
	HRESULT result;
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuff_->Map(0, nullptr, (void**)&constMapData);//マッピング
	assert(SUCCEEDED(result));

	constBuff_->Unmap(0, nullptr);

	return constBuff_;
}

void Sprite::StaticInitialize()
{
	keys_ = KeyInput::GetInstance();
	spriteCommon_ = SpriteCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();
}

void Sprite::Initialize(const std::string fileName,Vector2 position)
{
	directXBasic_ = spriteCommon_->GetDirectXBasic();

	winWide_ = static_cast<float>(directXBasic_->GetWinWidth());
	winHeight_ = static_cast<float>(directXBasic_->GetWinHeight());

	//定数バッファの生成
	constBuffMaterial_ = CrateConstBuff<SpriteCommon::ConstBufferDataMaterial, DirectXBasic>(constMapMaterial_, directXBasic_);
	constBuffTransform_ = CrateConstBuff<SpriteCommon::ConstBufferDataTransform, DirectXBasic>(constMapTransform_, directXBasic_);

	//単位行列を代入
	constMapTransform_->mat = MatrixIdentity();

	constMapTransform_->mat.m[0][0] = 2.0f / directXBasic_->GetWinWidth();		//ウインドウ横幅
	constMapTransform_->mat.m[1][1] = -2.0f / directXBasic_->GetWinHeight();	//ウインドウ縦幅
	//画面半分の平行移動
	constMapTransform_->mat.m[3][0] = -1.0f;
	constMapTransform_->mat.m[3][1] = 1.0f;

	scale_ = { 1.0f,1.0f,1.0f };
	rotation_ = { 0.0f };
	moveSpeed_ = { 0.0f,0.0f };
	anchorPoint_ = { 0.0f,0.0f };
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//テクスチャサイズをイメージに合わせる
	AdjustTextureSize(fileName);
	
	//テクスチャサイズをスプライトのサイズに適用
	size_ = textureSize_;

	//size_ = TextureManager::GetInstance()->GetTexSize(fileName);

	//ウインドウの中心に表示
	position_ = position;

	//スプライトの座標
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


	textureBuffer_ = TextureManager::GetInstance()->GetTextureBuffer(fileName);
	//もしその画像が読み込み済み
	if(textureBuffer_)
	{
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = textureBuffer_->GetDesc();

		float texLeft = textureLeftTop_.x / resDesc.Width;
		float texRight = (textureLeftTop_.x + textureSize_.x / resDesc.Width);
		float texTop = textureLeftTop_.y / resDesc.Height;
		float texBottom = (textureLeftTop_.y + textureSize_.y / resDesc.Height);
		//頂点のUVに反映する
		vertices_[LB].uv = { texLeft,texBottom };		//左下
		vertices_[LT].uv = { texLeft,texTop };			//左上
		vertices_[RB].uv = { texRight,texBottom };		//左下
		vertices_[RT].uv = { texRight,texTop };			//左上
	}

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	//カラーの書き込みと転送
	constMapMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	HRESULT result;
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;

	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for(int32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

#pragma region 頂点バッファビューの作成

	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion

	// 射影行列計算
	matProjection_ =
		Matrix4Orthographic(
			0.0f, WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, 0.0f, 0.0f, 1.0f);
}

void Sprite::matUpdate()
{
	//アンカーポイントの設定
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//左右反転
	if(isFlipX_ == true)
	{
		left = -left;
		right = -right;
	}
	//上下反転
	if(isFlipY_ == true)
	{
		top = -top;
		bottom = -bottom;
	}

	//頂点データ
	vertices_[LB].pos = { left,bottom ,0.0f };
	vertices_[LT].pos = { left,top ,0.0f };
	vertices_[RB].pos = { right,bottom ,0.0f };
	vertices_[RT].pos = { right,top ,0.0f };

	// UV座標
	if(textureBuffer_)
	{
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = textureBuffer_->GetDesc();

		float texLeft = textureLeftTop_.x / resDesc.Width;
		float texRight = (textureLeftTop_.x + textureSize_.x / resDesc.Width);
		float texTop = textureLeftTop_.y / resDesc.Height;
		float texBottom = (textureLeftTop_.y + textureSize_.y / resDesc.Height);
		//頂点のUVに反映する
		vertices_[LB].uv = { texLeft,texBottom };		//左下
		vertices_[LT].uv = { texLeft,texTop };			//左上
		vertices_[RB].uv = { texRight,texBottom };		//左下
		vertices_[RT].uv = { texRight,texTop };			//左上
	}

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	HRESULT result;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for(int32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // 座標をコピー
	}

	Matrix4 matScale;	//スケーリング行列
	matScale = MatrixScale(scale_);

	Matrix4 matRot;	//回転行列
	matRot = MatrixIdentity();
	matRot *= MatrixRotateZ(rotation_);	//Z軸周りに回転

	Matrix4 matTrans;	//平行移動行列
	matTrans = MatrixIdentity();
	matTrans *= MatrixTranslate(Vector3(position_.x, position_.y, 0.0f));	//平行移動

	//ワールド変換行列
	Matrix4 matWorld;
	matWorld = MatrixIdentity();	//単位行列を代入して変形をリセット
	matWorld *= matScale;
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
	//定数バッファにデータ転送
	constMapTransform_->mat = matWorld * matProjection_;

	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);//マッピング
	assert(SUCCEEDED(result));

	constMapMaterial_->color = color_;

	constBuffMaterial_->Unmap(0, nullptr);


	moveSpeed_.x = 0.0f;
	moveSpeed_.y = 0.0f;
}

void Sprite::Draw(const std::string& fileName)
{
	spriteCommon_->Update();

	uint32_t textureIndex;
	textureIndex = textureManager_->GetTextureMap().at(fileName);

	//頂点バッファビューの設定コマンド
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* heaps[] = { textureManager_->GetSRVHeap() };

	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//GPUのSRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//取得したサイズを使用してハンドルを進める
	for(uint32_t i = 0; i < textureIndex; i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	if(isInvisible_)
	{
		return;
	}

	//描画コマンド(頂点数、インスタンスの数、最初の頂点のインデックス,データを読み取る前に各インデックスに追加される値)
	directXBasic_->GetCommandList()->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);
}

void Sprite::TransferVertices()
{
	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;
	if(isFlipX_)
	{ // 左右入れ替え
		left = -left;
		right = -right;
	}

	if(isFlipY_)
	{ // 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	Vertex vertices[kVertexCount_];

	vertices[LB].pos = { left, bottom, 0.0f };  // 左下
	vertices[LT].pos = { left, top, 0.0f };     // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top, 0.0f };    // 右上

	// 頂点バッファへのデータ転送
	memcpy(vertMap_, vertices, sizeof(vertices));
}

void Sprite::Update()
{
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
}

void Sprite::MovePos(Vector2 moveVec)
{
	position_ += moveVec;
	matUpdate();
}

void Sprite::AdjustTextureSize(std::string fileName)
{
	ID3D12Resource* textureBuffer = TextureManager::GetInstance()->GetTextureBuffer(fileName);

	//テクスチャ情報取得
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}
