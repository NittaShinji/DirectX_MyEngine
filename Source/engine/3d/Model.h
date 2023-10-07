#pragma once
#include "DirectXBasic.h"
#include "Mesh.h"
#include <map>
#include <array>
#include "Vector2.h"
#include "Matrix4.h"

class Model
{
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
		Vector3 ambient;	//アンビエント影響度
		Vector3 diffuse;	//ディフューズ影響度
		Vector3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material()
		{
			ambient = { 1.0f,1.0f,1.0f };
			diffuse = { 1.0f,1.0f,1.0f };
			specular = { 1.0f,1.0f, 1.0f };
			alpha = 1.0f;
		}
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferDateB0
	{
		Matrix4 mat;	//3D変換行列

	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDateB1
	{
		Vector3 ambient;	//アンビエント係数
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
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

		//メッシュ
		std::vector<Mesh> meshes;

		//SRV用のデスクリプタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

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
	const std::vector<Mesh>& GetMeshes() { return infomation_.meshes; }

	//セッター
	void SetName(const MODELKEY& name) { name_ = name; };
	void SetInfomation(const MODELVALUE& infomation) { infomation_ = infomation; };
};

