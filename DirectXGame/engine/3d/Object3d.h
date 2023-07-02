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
	//nameSpace
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//Object3d(DirectXBasic* directXBasic,XMFLOAT3 position);
	Object3d();
	~Object3d();

	static void StaticInitialize(DirectXBasic* directXBasic);
	void Initialize(const std::string& path, const XMFLOAT3& Modelscale, const XMFLOAT3& position, const XMFLOAT3& rotation);
	void Update(Camera* camera);
	//void BeforeDraw();
	static void BeforeDraw();

	void AfterDraw();
	void Draw();
	void SetModel(const std::string& path);
	void Create(Model* model);
	void CrateConstBuffandMapping();

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static LightGroup* lightGroup_;
	Model model_;
	Sprite* sprite_;
	Camera* camera_ = nullptr;
	
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
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffLight_;
	
	//定数バッファのマッピング用ポインタ
	ConstBufferDateTransform* constMapTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	ConstBufferLightDate* constMapLight_ = nullptr;

	//スケール
	XMFLOAT3 scale_;
	//回転角
	XMFLOAT3 rotation_;
	//平行移動
	XMFLOAT3 transform_;

	XMMATRIX matScale_, matRot_, matTrans_;

	//ワールド変換行列
	XMMATRIX matWorld_;

	//ビュー行列
	XMMATRIX matView_;
	//射影行列
	XMMATRIX matProjection_;
	//カメラ座標
	XMFLOAT3 cameraPos_;

	//親オブジェクトのポインタ
	
	ID3DBlob* vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob_ = nullptr; // エラーオブジェクト

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	//グラフィックスパイプライン
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;

	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	bool colorFlag_ = false;

	static const UINT kRenderTexNum = 2;

public:

	//ゲッター
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform_; };

	XMFLOAT3 GetWorldPos() const ;
	void SetTransform(const XMFLOAT3& pos) { transform_ = pos; };
	void SetRotation(const XMFLOAT3& rotate) { rotation_ = rotate; };
	void SetScale(const XMFLOAT3& scale) { scale_ = scale; }

	void SetMatTrans(const XMMATRIX& matTrans) { matTrans_ = matTrans; }
	void SetMatRot(const XMMATRIX& matRot) { matRot_ = matRot; }
	void SetMatScale(const XMMATRIX& matScale) { matScale_ = matScale; }


	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }

	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);


public: //静的メンバ関数

	static void SetLightGroup(LightGroup* lightGroup) { Object3d::lightGroup_ = lightGroup; }

};
