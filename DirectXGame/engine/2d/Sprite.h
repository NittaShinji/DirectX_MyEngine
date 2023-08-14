#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
#include "Input.h"

//スプライト
class Sprite
{
private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	
	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize(Vector2 position, Vector2 size);
	//行列更新
	void matUpdate();
	//描画
	void Draw(const std::string& fileName);

	//void TexMapping();

	void TransferVertices();

	void Update();

	//テンプレートコンストラクタ
	template <typename Type1, typename Type2>
	//定数バッファの生成
	ComPtr<ID3D12Resource> CrateConstBuff(Type1*& constMapData, Type2* directXBasic_);

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
	//Vector3 position;
	Vector2 moveSpeed_;
	//色(RGBA)
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//表示サイズ
	Vector2 size_ = { 100.0f,100.0f };

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
	
	//初期ポジション
	Vector2 position_;

	//頂点配列
	std::array<Vertex, kVertexCount_> vertices_{};

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	
	// 頂点データ全体のサイズ
	HRESULT result_;
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	//テクスチャマッピング用の変数
	//横方向ピクセル数
	//const size_t kTextureWidth_ = 256;
	//縦方向ピクセル数
	//const size_t kTextureHeight_ = 256;
	//配列の要素数
	//const size_t imageDateCount_ = kTextureWidth_ * kTextureHeight_;
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
	//ID3D12Resource* constBuffMaterial = nullptr;
	//座標用の定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//定数バッファのGPUリソースのポインタ
	SpriteCommon::ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	//定数バッファのマッピング用ポインタ
	SpriteCommon::ConstBufferDataTransform* constMapTransform_ = nullptr;


public:

	std::array <Vertex, kVertexCount_> GetterVertex() { return vertices_; };

	//ゲッター
	const Vector2& GetMoveSpeed_() const { return moveSpeed_; };
	float GetRotation() const { return rotation_; };
	const Vector4& GetColor() const { return color_; };
	const Vector2& GetSize() const { return size_; };
	const Vector2& GetAnchorPoint() const { return anchorPoint_; };
	const Vector2& GetPosition() const { return position_; };
	bool GetIsFlipX() const { return isFlipX_; };
	bool GetIsFlipY() const { return isFlipY_; };
	bool GetIsInvisible() const { return isInvisible_; };

	//セッター
	void SetMoveSpeed_(const Vector2& moveSpeed) { moveSpeed_ = moveSpeed; };
	void SetRotation(float rotation) { rotation_ = rotation; };
	void SetColor(const Vector4& color) { color_ = color; };
	void SetSize(const Vector2& size) { size_ = size; };
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetPosition(const Vector2& initPosition) { position_ = initPosition; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	void SetWidth(UINT width) { width_ = width; }
	void SetHeight(UINT height) { height_ = height; }
};
