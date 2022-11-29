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

private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;

	//struct Vertex
	//{
	//	XMFLOAT3 pos;		//xyz座標
	//	XMFLOAT3 normal;	//法線ベクトル
	//	XMFLOAT2 uv;		//画像内のxyz座標
	//};

	struct VertexPos
	{
		XMFLOAT3 pos;
	};

	//頂点の数
	static const int vertexCount = 3;
	//画像枚数
	static const int imageCount = 2;

	//頂点配列
	std::array<VertexPos, vertexCount> vertices{};
	
	// 頂点バッファビューの作成
	//std::array <D3D12_VERTEX_BUFFER_VIEW, imageCount> vbView{};
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	
	// 頂点データ全体のサイズ
	
	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12RootSignature* rootSignature;

public:

	std::array <VertexPos, vertexCount> GetterVertex() { return vertices; };

};
