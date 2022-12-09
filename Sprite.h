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

	//初期化
	void Initialize(SpriteCommon* spriteCommon);
	
	void matUpdate();
	void Update();

	void ImageDateSet();
	void ImageDateSRVSet();

	void TexMapping();
	void TexMappingSRVSet();

	/*void PointListUpdate();
	void LineListUpdate();*/


private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;
	KeyInput* keys_ = nullptr;

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

	struct Vertex
	{
		XMFLOAT3 pos;// xyz座標
		XMFLOAT2 uv; // uv座標
	};

	//射影変換行列
	XMMATRIX matProjection;

	//スケーリング倍率
	XMFLOAT3 scale;
	//回転角
	float rotationZ;
	//座標
	XMFLOAT3 position;

	//頂点の数
	static const int vertexCount = 4;
	
	//頂点配列
	std::array<Vertex, vertexCount> vertices{};


	
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

	//テクスチャバッファ
	ID3D12Resource* texBuff = nullptr;

	//テクスチャリソースデスク
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_RESOURCE_DESC resDesc{};

	//シェーダーリソースビュー
	ID3D12DescriptorHeap* srvHeap = nullptr;

public:

	std::array <Vertex, vertexCount> GetterVertex() { return vertices; };

	//ゲッター
	ID3D12Resource* GetTexBuff() { return texBuff; };

	//ID3D12DescriptorHeap* GetSRVheap() { return srvHeap; };
};
