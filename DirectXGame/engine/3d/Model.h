#pragma once
#include <string>
#include <Vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <map>
#include <array>
#include "DirectXBasic.h"

class Model
{

private:

	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void Load(const std::string& path);
	void Update();
	
	static void StaticInitialize(DirectXBasic* directXBasic);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	static void LoadMaterial(const std::string& directoryPath, const std::string& fileName, Model& model);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	static void LoadTexture(const std::string& directoryPath, const std::string& fileName, Model& model);

private:

	static DirectXBasic* directXBasic_;

	struct Material
	{
		std::string name;	//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		// uv座標
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferDateB0
	{
		XMMATRIX mat;	//3D変換行列
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDateB1
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public:

	using MODELKEY = std::string;

	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 256;

	struct MODELVALUE
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	//定数バッファ

		//定数バッファのマッピング用ポインタ
		ConstBufferDateB0* constMapTransform = nullptr;

		//定数バッファ用データ
		ConstBufferDateB1* constMap1 = nullptr;

		//マテリアル
		Material material;

		//頂点データ配列
		std::vector<Vertex> vertices;
		//頂点インデックス配列
		std::vector<unsigned short> indices;

		//頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		//インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
		//SRV用のデスクリプタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
		
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		//インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView{};

		//テクスチャバッファ
		std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers;
	};	

	static const MODELVALUE* GetMODELVALUE(const MODELKEY path);

private:

	//デスクリプタヒープハンドル
	static D3D12_CPU_DESCRIPTOR_HANDLE sSrvHandle_;

	//テクスチャ番号
	static uint32_t sTextureIndex_;

	MODELKEY name_;
	MODELVALUE infomation_;
	static std::map<MODELKEY, MODELVALUE> sModels_;

public:

	//ゲッター
	MODELKEY* GetName() { return &name_; };
	MODELVALUE* GetInfomation() { return &infomation_; };
	uint32_t const GetTexIndex() { return sTextureIndex_; };

	//セッター
	void SetName(const MODELKEY& name) { name_ = name; };
	void SetInfomation (const MODELVALUE& infomation) { infomation_ = infomation; };
};

