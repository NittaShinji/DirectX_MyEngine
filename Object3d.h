#pragma once
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
using namespace DirectX;

class Object3d
{
public:
	
	static void StaticInitialize(Microsoft::WRL::ComPtr<ID3D12Device> device, int window_width, int window_hieght);

	void Create();
	void CreateModel();
	void InitializeObject3d(DirectXBasic* directXBasic);
	void Initialize();
	void Update();
	void Draw();

	void RootSignatureSet();

private:

	DirectXBasic* directXBasic_ = nullptr;

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//色(RGBA)
	};
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;	
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		// uv座標
	};


	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye{ 0, 0, -100 };	//視点座標
	XMFLOAT3 target{0, 0, 0};	//注視点座標
	XMFLOAT3 up{ 0, 1, 0 };		//上方向ベクトル

	//射影変換行列
	XMMATRIX matProjection;

	////スケーリング倍率
	//XMFLOAT3 scale;
	////回転角
	//XMFLOAT3 rotation;
	////座標
	//XMFLOAT3 position;

	//色用の定数バッファ
	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	
	//定数バッファ(行列用)
	ID3D12Resource* constBuffTransform;
	//定数バッファマップ(行列用)
	ConstBufferDataTransform* constMapTransform;
	//アフィン変換情報
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matworld;
	//親オブジェクトのポインタ
	Object3d* parent = nullptr;

	// 3Dオブジェクトの数
	static const size_t kObjectCount = 50;
	// 3Dオブジェクトの配列
	//Object3d object3ds[kObjectCount];

	//頂点配列
	//std::vector<Vertex> vertices_{};

	//XMFLOAT3 vertices_{};

	//uint16_t indices[] =
	//{
	//	0,1,2, //三角形1つ目
	//	1,2,3, //三角形2つ目
	//};

	HRESULT result_;
	//頂点データ配列
	static std::vector<Vertex> vertices_;
	//頂点インデックス配列
	static std::vector<unsigned short> indices_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//テクスチャリソースデスク
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_RESOURCE_DESC resDesc{};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

};
