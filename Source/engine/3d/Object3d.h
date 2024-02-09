#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include "Vector3.h"

namespace NsEngine
{
	class BaseCollider;
}
/// <summary>
/// オブジェクト
/// 当たり判定をつけないこともできる
/// </summary>
class Object3d
{
private: //エイリアス

	//テンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //構造体

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

public: //メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

	//静的インスタンスを作成
	static std::unique_ptr<Object3d> Create(const std::string& path);
	//静的初期化
	static void StaticInitialize(DirectXBasic* directXBasic);

	//初期化
	virtual void Initialize();
	//更新
	virtual void Update(Camera* camera);
	//描画前処理
	static void BeforeDraw();
	//描画
	virtual void Draw();

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="path">ファイルパス</param>
	void SetModel(const std::string& path);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info);

	/// <summary>
	/// オブジェクトを移動させる関数
	/// </summary>
	/// <param name="moveVec">移動量</param>
	void MovePos(Vector3 moveVec);

	//行列の更新
	void UpdateWorldMatrix();

	//グラフィックスパイプラインの初期化
	static void InitializeGraphicsPipeline();

	//コライダーを削除する
	void RemoveCollider();

protected:	//メンバ変数

	//クラス名(デバッグ用)
	const char* name = nullptr;
	//コライダー
	NsEngine::BaseCollider* collider_ = nullptr;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static LightGroup* lightGroup_;
	Model model_;
	Camera* camera_ = nullptr;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_;

	//定数バッファのマッピング用ポインタ
	ConstBufferDateTransform* constMapTransform_;
	ConstBufferDataMaterial* constMapMaterial_;

	//前フレームスケール
	Vector3 oldScale_;
	//前フレーム回転角
	Vector3 oldRotation_;
	//前フレーム平行移動
	Vector3 oldTransform_;

	//行列更新フラグ
	bool isChangedMatrix_;

	//スケール
	Vector3 scale_;
	//回転角
	Vector3 rotation_;
	//平行移動
	Vector3 transform_;

	//スケール、回転、平行移動変換行列
	Matrix4 matScale_, matRot_, matTrans_;

	//ワールド変換行列
	Matrix4 matWorld_;

	//ビュー行列
	Matrix4 matView_;

	//射影行列
	Matrix4 matProjection_;

	//カメラ座標
	Vector3 cameraPos_;

	ID3DBlob* vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* gsBlob_ = nullptr;	//ジオメトリシェーダーオブジェクト
	ID3DBlob* errorBlob_ = nullptr; // エラーオブジェクト

	//パイプラインステート
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
	//ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	//グラフィックスパイプライン
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_;

	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;

	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	//レンダーテクスチャの数
	static const UINT kRenderTexNum = 2;

	//色変えフラグ
	bool colorFlag_ = false;
	//透明フラグ
	bool alphaFlag_ = false;

	//色
	const Vector3 kDefaultColor_ = { 1.0f,1.0f,1.0f };
	Vector3 color_ = kDefaultColor_;
	//透明度
	const float kDefaultAlpha_ = 1.0f;
	float alpha_ = kDefaultAlpha_;
	//属性
	int32_t attributeColor_;

	//初期位置
	Vector3 initPos_;

public: //アクセッサ

	//ゲッター
	ConstBufferDateTransform* GetConstMapTransform() { return constMapTransform_; };

	Vector3 GetWorldPos() const;
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4& GetMatWorld() { return matWorld_; }

	//モデルを取得
	Model* GetModel() { return &model_; }
	//色変えフラグを取得
	bool GetColorFlag() { return colorFlag_; }
	//属性を取得
	int32_t GetAttributeColor() { return attributeColor_; }
	//スケールを取得
	Vector3 GetScale() { return scale_; }
	//回転角を取得
	Vector3 GetRotation() { return rotation_; }
	//平行移動を取得
	Vector3 GetTransform() { return transform_; }
	//初期位置を取得
	Vector3 GetInitPos() { return initPos_; }

	//平行移動をセット
	void SetTransform(const Vector3& pos) { transform_ = pos; };
	//回転角をセット
	void SetRotation(const Vector3& rotate) { rotation_ = rotate; };
	//スケールをセット
	void SetScale(const Vector3& scale) { scale_ = scale; }

	//平行移動行列をセット
	void SetMatTrans(const Matrix4& matTrans) { matTrans_ = matTrans; }
	//回転行列をセット
	void SetMatRot(const Matrix4& matRot) { matRot_ = matRot; }
	//スケール行列をセット
	void SetMatScale(const Matrix4& matScale) { matScale_ = matScale; }
	//色変えフラグをセット
	void SetColorFlag(bool colorFlag) { colorFlag_ = colorFlag; }
	//色をセット
	void SetColor(Vector3 color) { color_ = color; }
	//アンビエントカラーをセット
	void SetAmbient(Vector3 color);
	//属性色をセット
	void SetAttributeColor(int32_t attribute) { attributeColor_ = attribute; }
	//透明フラグをセット
	void SetAlphaFlag(bool alphaFlag) { alphaFlag_ = alphaFlag; }
	//透明度をセット
	void SetAlpha(float alpha) { alpha_ = alpha; }
	//初期位置をセット
	void SetInitPos(Vector3 initPos) { initPos_ = initPos; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(NsEngine::BaseCollider* collider);

	//テンプレートコンストラクタ
	template <typename Type1>
	//定数バッファの生成
	static ComPtr<ID3D12Resource> CrateConstBuff(Type1* directXBasic_);


public: //アクセッサ(静的メンバ関数)

	//ライトグループをセット
	static void SetLightGroup(LightGroup* lightGroup) { Object3d::lightGroup_ = lightGroup; }

};
