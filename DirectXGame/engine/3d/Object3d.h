#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include <vector>
#include <string>
#include "Vector3.h"

class BaseCollider;

class Object3d
{
private: //エイリアス

	//テンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

	//定数バッファ用データ構造体(座標系)b0
	struct ConstBufferDateTransform
	{
		Matrix4 viewProjection;
		//ワールド行列
		Matrix4 worldMatrix;
		//カメラ座標(ワールド行列)
		Vector3 cameraPos;
		float pad1;
	};

	//定数バッファ用データ構造体(マテリアル)b1
	struct ConstBufferDataMaterial
	{
		Vector3 ambient;	//アンビエント係数
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};

	struct GSOutput
	{
		Vector3 pos;		// xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;		// uv座標
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

	static std::unique_ptr<Object3d> Create(const std::string& path);

	static void StaticInitialize(DirectXBasic* directXBasic);
	virtual void Initialize();
	virtual void Update(Camera* camera);
	static void BeforeDraw();

	void AfterDraw();
	virtual void Draw();
	void SetModel(const std::string& path);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

protected:	//メンバ変数

	//クラス名(デバッグ用)
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static LightGroup* lightGroup_;
	Model model_;
	Camera* camera_ = nullptr;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_;

	//定数バッファのマッピング用ポインタ
	ConstBufferDateTransform* constMapTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	//スケール
	Vector3 scale_;
	//回転角
	Vector3 rotation_;
	//平行移動
	Vector3 transform_;

	Matrix4 matScale_, matRot_, matTrans_;

	//ワールド変換行列
	Matrix4 matWorld_;

	//ビュー行列
	Matrix4 matView_;

	//射影行列
	Matrix4 matProjection_;

	//カメラ座標
	Vector3 cameraPos_;

	//親オブジェクトのポインタ

	ID3DBlob* vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* gsBlob_ = nullptr;	//ジオメトリシェーダーオブジェクト
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

	Vector3 color_ = { 1,1,1};

	//属性
	int32_t attribute_;

public:

	//ゲッター
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform_; };

	Vector3 GetWorldPos() const;
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4& GetMatWorld() { return matWorld_; }

	//モデルを取得
	//const Model& GetModel() { return model_; }
	Model* GetModel() { return &model_; }
	bool GetColorFlag() { return colorFlag_; }
	int32_t GetAttribute() { return attribute_; }


	void SetTransform(const Vector3& pos) { transform_ = pos; };
	void SetRotation(const Vector3& rotate) { rotation_ = rotate; };
	void SetScale(const Vector3& scale) { scale_ = scale; }

	void SetMatTrans(const Matrix4& matTrans) { matTrans_ = matTrans; }
	void SetMatRot(const Matrix4& matRot) { matRot_ = matRot; }
	void SetMatScale(const Matrix4& matScale) { matScale_ = matScale; }

	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }
	void SetColor(Vector3 color) { color_ = color; }
	void SetAmbient(Vector3 color);
	void SetAttribute(int32_t attribute) { attribute_ = attribute; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);


public: //静的メンバ関数

	static void SetLightGroup(LightGroup* lightGroup) { Object3d::lightGroup_ = lightGroup; }

};
