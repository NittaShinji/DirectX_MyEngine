#pragma once
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <string>

#include "Model.h"
#include "Sprite.h"

using namespace DirectX;

class Object3d
{
public:

	Object3d(const std::string& path, DirectXBasic* directXBasic, Sprite* sprite);

	void Update();
	void BeforeDraw();
	void AfterDraw();
	void Draw(ID3D12DescriptorHeap* srvHeapHandle);

private:

	DirectXBasic* directXBasic_ = nullptr;
	Model model_;
	Sprite* sprite_;

	//定数バッファ用データ構造体B1
	//struct ConstBufferDateB1
	//{
	//	XMFLOAT3 ambient;	//アンビエント係数
	//	float pad1;
	//	XMFLOAT3 diffuse;
	//	float pad2;
	//	XMFLOAT3 specular;
	//	float alpha;
	//};
	
	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//色(RGBA)
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferDateB0
	{
		XMMATRIX mat;	//3D変換行列
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		// uv座標
	};
	
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	//定数バッファのマッピング用ポインタ
	ConstBufferDateB0* constMap0 = nullptr;

	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 transform;
	
	std::vector<Vertex> vertex_;

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーリソースビュー
	ID3D12DescriptorHeap* srvHeap = nullptr;


};
