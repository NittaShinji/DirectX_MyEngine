#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
using namespace DirectX;

//スプライト
class Sprite
{
public:

	//初期化
	void Initialize(SpriteCommon* spriteCommon);
	
	void Update();

	void ImageDateSet();

	void PointListUpdate();
	void LineListUpdate();


private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;

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
	
	



public:

	std::array <Vertex, vertexCount> GetterVertex() { return vertices; };

};
