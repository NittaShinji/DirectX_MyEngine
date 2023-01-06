#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include <vector>
#include <string>

using namespace DirectX;

class Object3d
{
public:

	//Object3d(DirectXBasic* directXBasic,XMFLOAT3 position);
	Object3d(const std::string& path,XMFLOAT3 position,Model model);

	static void StaticInitialize(DirectXBasic* directXBasic);

	void Update();
	void BeforeDraw();
	void AfterDraw();
	void Draw();
	void LoadTexture(uint32_t textureIndex, const std::string& fileName);
	void SetModel(const std::string& path);

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
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
		//XMFLOAT4 color;	//色(RGBA)
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferDateTransform
	{
		XMMATRIX mat;	//3D変換行列
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		// uv座標
	};
	
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;

	
	//定数バッファのマッピング用ポインタ
	ConstBufferDateTransform* constMapTransform = nullptr;
	//ConstBufferDataMaterial* constMapColor = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//スケール
	XMFLOAT3 scale;
	//回転角
	XMFLOAT3 rotation;
	//平行移動
	XMFLOAT3 transform;

	//親オブジェクトのポインタ
	
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーリソースビュー
	ID3D12DescriptorHeap* srvHeap = nullptr;

	////SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	////テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	//テクスチャリソースデスク
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC resDesc{};

	//テクスチャ番号
	uint32_t textureIndex_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

public:

	//ゲッター
	//uint32_t GetTextureIndex() const { return textureIndex_; };

	//セッター
	//void SetTextureIndex(uint32_t textureIndex) { textureIndex_ = textureIndex; };

};
