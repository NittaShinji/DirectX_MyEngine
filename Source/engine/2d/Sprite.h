#pragma once
#include "SpriteCommon.h"
#include "Input.h"

/// <summary>
/// スプライト・画像クラス
/// </summary>
class Sprite
{
private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:

	enum VertexNumber
	{
		LB,//右上
		LT,//左上
		RB,//右下
		RT,//右上
	};

	struct Vertex
	{
		Vector3 pos;// xyz座標
		Vector2 uv; // uv座標
	};

public:

	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize(const std::string fileName, Vector2 position);
	//行列更新
	void matUpdate();
	//描画
	void Draw(const std::string& fileName);

	//行列を送る
	void TransferVertices();

	//テンプレートコンストラクタ
	template <typename Type1, typename Type2>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1*& constMapData, Type2* directXBasic_);

	//移動させる関数
	void MovePos(Vector2 moveVec);

private:

	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize(std::string fileName);

protected:

	static TextureManager* textureManager_;
	static SpriteCommon* spriteCommon_;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;

	//ウインドウの長さ
	float winWide_;
	float winHeight_;

	//スプライトの長さ
	UINT width_;
	UINT height_;

	//スケーリング倍率
	Vector3 scale_;
	//回転角
	float rotation_;
	//座標
	Vector2 position_;
	//色(RGBA)
	Vector4 color_ ;
	//表示サイズ
	const Vector2 kDefaultSize = { 100.0f,100.0f };
	Vector2 size_ = kDefaultSize;

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;

	//頂点の数
	static const int32_t kVertexCount_ = 4;

	//アンカーポイント
	Vector2 anchorPoint_;

	//非表示フラグ
	bool isInvisible_ = false;

	//頂点配列
	std::array<Vertex, kVertexCount_> vertices_{};

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// 頂点データ全体のサイズ
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	//画像イメージデータ配列
	Vector4* imageDate_;
	//テクスチャリソースデスク
	D3D12_RESOURCE_DESC textureResourceDesc_{};

	D3D12_RESOURCE_DESC resDesc_{};

	// 射影行列
	static Matrix4 matProjection_;

	// 頂点バッファマップ
	Vertex* vertMap_ = nullptr;

	//色用の定数バッファ
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//座標用の定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//定数バッファのGPUリソースのポインタ
	SpriteCommon::ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	//定数バッファのマッピング用ポインタ
	SpriteCommon::ConstBufferDataTransform* constMapTransform_ = nullptr;

	//テクスチャバッファ
	ID3D12Resource* textureBuffer_ = nullptr;
	//テクスチャ左上座標
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureClipSize_ = { 100.0f,100.0f };

public:

	//ゲッター
	//座標を取得
	const Vector2& GetPosition() const { return position_; };
	//回転角を取得
	float GetRotation() const { return rotation_; };
	//サイズを取得
	const Vector2& GetSize() const { return size_; };
	//色を取得
	const Vector4& GetColor() const { return color_; };
	//アンカーポイントを取得
	const Vector2& GetAnchorPoint() const { return anchorPoint_; };
	//左右入れ替えフラグを取得
	bool GetIsFlipX() const { return isFlipX_; };
	//上下入れ替えフラグを取得
	bool GetIsFlipY() const { return isFlipY_; };
	//非表示フラグを取得
	bool GetIsInvisible() const { return isInvisible_; };
	//テクスチャ左上座標を取得
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; };
	
	//セッター
	//座標をセット
	void SetPosition(const Vector2& initPosition) { position_ = initPosition; }
	//回転角をセット
	void SetRotation(float rotation) { rotation_ = rotation; };
	//サイズをセット
	void SetSize(const Vector2& size) { size_ = size; };
	//色をセット
	void SetColor(const Vector4& color) { color_ = color; };
	//アンカーポイントをセット
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	//左右入れ替えフラグをセット
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	//上下入れ替えフラグをセット
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	//非表示フラグをセット
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	//テクスチャ左上座標をセット
	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; };
	//切り抜くサイズをセット
	void SetTextureClipSize(const Vector2& textureSize) { textureClipSize_ = textureSize; size_ = textureSize; };
};
