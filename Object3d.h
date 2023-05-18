#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "LightGroup.h"
#include <vector>
#include <string>

class Object3d
{
	//エイリアステンプレート
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//Object3d(DirectXBasic* directXBasic,XMFLOAT3 position);
	Object3d(const std::string& path,XMFLOAT3 position, XMFLOAT3 Modelscale);
	~Object3d();

	static void StaticInitialize(DirectXBasic* directXBasic);
	void Initialize();
	void Update(Camera* camera);
	void BeforeDraw();
	void AfterDraw();
	void Draw();
	void SetModel(const std::string& path);

	void CrateConstBuffandMapping();

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static LightGroup* lightGroup_;
	Model model_;
	Sprite* sprite_;
	Camera* camera_ = nullptr;

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
	
	//定数バッファ用データ構造体b0
	struct ConstBufferDateTransform
	{
		//XMMATRIX mat;	//3D変換行列

		XMMATRIX viewProjection;
		//ワールド行列
		XMMATRIX worldMatrix;
		//カメラ座標(ワールド行列)
		XMFLOAT3 cameraPos;
		float pad1;
		//XMFLOAT4 color;	//色(RGBA)
	};

	//定数バッファ用データ構造体(マテリアル)b1
	struct ConstBufferDataMaterial
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

	//定数バッファ用データ構造体(ライト情報)b2
	struct ConstBufferLightDate
	{
		XMFLOAT3 lightv;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
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
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffLight;
	
	//定数バッファのマッピング用ポインタ
	ConstBufferDateTransform* constMapTransform = nullptr;
	//ConstBufferDataMaterial* constMapColor = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferLightDate* constMapLight = nullptr;

	//スケール
	XMFLOAT3 scale;
	//回転角
	XMFLOAT3 rotation;
	//平行移動
	XMFLOAT3 transform;

	XMMATRIX matScale, matRot, matTrans;

	//ワールド変換行列
	XMMATRIX matWorld;

	//ビュー行列
	XMMATRIX matView_;
	//射影行列
	XMMATRIX matProjection_;
	//カメラ座標
	XMFLOAT3 cameraPos;

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

	bool colorFlag_ = false;

public:

	//ゲッター
	//uint32_t GetTextureIndex() const { return textureIndex_; };
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform; };

	XMFLOAT3 GetWorldPos();
	void SetTransform(XMFLOAT3 pos) { transform = pos; };
	void SetRotation(XMFLOAT3 rotate) { rotation = rotate; };

	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }

	//テンプレートコンストラクタ
	template <typename Type1, typename Type2, typename Type3>
	//定数バッファの生成
	void CrateConstBuff(Type1*& constBuffer, Type3* directXBasic_);

	//セッター
	//void SetTextureIndex(uint32_t textureIndex) { textureIndex_ = textureIndex; };

public: //静的メンバ関数

	static void SetLightGroup(LightGroup* lightGroup) { Object3d::lightGroup_ = lightGroup; }

};
