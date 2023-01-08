#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
#include <DirectXTex.h>
#include "Input.h"
using namespace DirectX;

//スプライト
class Sprite
{
public:

	static void StaticInitialize(SpriteCommon* spriteCommon);

	//初期化
	void Initialize(XMFLOAT2 position, XMFLOAT2 size, uint32_t textureIndex);
	
	void matUpdate();
	void Draw();

	void BeforeDraw();

	void ImageDateSet();
	void ImageDateSRVSet();

	static void LoadTexture(uint32_t index, const std::string& fileName);

	/*void TexMapping();
	void TexMappingSRVSet();*/

	/*void PointListUpdate();
	void LineListUpdate();*/


private:

	static SpriteCommon* spriteCommon_;
	static DirectXBasic* directXBasic_;
	
	//KeyInput* keys_ = nullptr;
	static KeyInput* keys_;

	//struct Vertex
	//{
	//	XMFLOAT3 pos;		//xyz座標
	//	XMFLOAT3 normal;	//法線ベクトル
	//	XMFLOAT2 uv;		//画像内のxyz座標
	//};

	/*enum VertexName
	{
		LEFTDOWN,
		LEFTCENTER,
		LEFTUP,
		RIGHT
	};*/

	enum VertexNumber
	{
		LB,//右上
		LT,//左上
		RB,//右下
		RT,//右上
	};

	struct Vertex
	{
		XMFLOAT3 pos;// xyz座標
		XMFLOAT2 uv; // uv座標
	};

	//ウインドウの長さ
	float winWide;
	float winHeight;

	//射影変換行列
	XMMATRIX matProjection;

	//スケーリング倍率
	XMFLOAT3 scale;
	//回転角
	float rotation_;
	//座標
	//XMFLOAT3 position;
	XMFLOAT2 moveSpeed_;
	//色(RGBA)
	XMFLOAT4 color_ = { 1,1,1,1 };
	//表示サイズ
	XMFLOAT2 size_ = { 100.0f,100.0f };

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;

	//頂点の数
	static const int vertexCount = 4;

	//アンカーポイント
	XMFLOAT2 anchorPoint_;

	//非表示フラグ
	bool isInvisible_ = false;
	
	//初期ポジション
	XMFLOAT2 position_;

	//頂点配列
	std::array<Vertex, vertexCount> vertices_{};

	// 頂点バッファビューの作成
	//std::array <D3D12_VERTEX_BUFFER_VIEW, imageCount> vbView{};
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	
	// 頂点データ全体のサイズ
	
	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	//横方向ピクセル数
	const size_t textureWidth = 256;
	//縦方向ピクセル数
	const size_t textureHeight = 256;
	//配列の要素数
	const size_t imageDateCount = textureWidth * textureHeight;

	//画像イメージデータ配列
	XMFLOAT4* imageDate;

	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	//テクスチャバッファ
	//ID3D12Resource* texBuff = nullptr;
	//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;
	static std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	//テクスチャリソースデスク
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_RESOURCE_DESC resDesc{};

	//シェーダーリソースビュー
	//ID3D12DescriptorHeap* srvHeap = nullptr;
	static ID3D12DescriptorHeap* srvHeap;

	//テクスチャ番号
	static uint32_t textureIndex_;
	//uint32_t textureIndex_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	//デスクリプタハンドルのサイズ
	//UINT incrementSize;
public:

	std::array <Vertex, vertexCount> GetterVertex() { return vertices_; };

	//ゲッター
	//ID3D12Resource* GetTexBuff() { return texBuff; };
	const XMFLOAT2& GetMoveSpeed_() const { return moveSpeed_; };
	float GetRotation() const { return rotation_; };
	const XMFLOAT4 GetColor() const { return color_; };
	const XMFLOAT2 GetSize() const { return size_; };
	const XMFLOAT2 GetAnchorPoint() const { return anchorPoint_; };
	const XMFLOAT2 GetPosition() const { return position_; };
	bool GetIsFlipX() const { return isFlipX_; };
	bool GetIsFlipY() const { return isFlipY_; };
	bool GetIsInvisible() const { return isInvisible_; };
	uint32_t GetTextureIndex() const { return textureIndex_; };
	ID3D12DescriptorHeap* GetSRVHeap() const { return srvHeap; };

	//セッター
	void SetMoveSpeed_(const DirectX::XMFLOAT2& moveSpeed) { moveSpeed_ = moveSpeed; };
	void SetRotation(float rotation) { rotation_ = rotation; };
	void SetColor(const DirectX::XMFLOAT4 color) { color_ = color; };
	//ID3D12DescriptorHeap* GetSRVheap() { return srvHeap; };
	void SetSize(const DirectX::XMFLOAT2 size) { size_ = size; };
	void SetAnchorPoint(const DirectX::XMFLOAT2 anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetPosition(const DirectX::XMFLOAT2 initPosition) { position_ = initPosition; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	void SetTextureIndex(uint32_t textureIndex) { textureIndex_ = textureIndex; };

};
