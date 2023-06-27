#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
#include "Input.h"
using namespace DirectX;

//スプライト
class Sprite
{
public:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//静的初期化
	static void StaticInitialize();

	//初期化
	void Initialize(XMFLOAT2 position, XMFLOAT2 size);
	//行列更新
	void matUpdate();
	//描画
	void Draw(const std::string& fileName);

	void ClearDepthBuffer();

	/*void TexMapping();
	void TexMappingSRVSet();*/

	/*void PointListUpdate();
	void LineListUpdate();*/

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
		XMFLOAT3 pos;// xyz座標
		XMFLOAT2 uv; // uv座標
	};

	static SpriteCommon* spriteCommon_;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;

	//ウインドウの長さ
	float winWide_;
	float winHeight_;

	//スプライトの長さ
	float wide_;
	float height_;

	//スケーリング倍率
	XMFLOAT3 scale_;
	//回転角
	float rotation_;
	//座標
	//XMFLOAT3 position;
	XMFLOAT2 moveSpeed_;
	//色(RGBA)
	XMFLOAT4 color_ = { 1,1,1,1 };
	//表示サイズ
	XMFLOAT2 size_ = { 100.0f,100.0f };

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;

	//頂点の数
	static const int32_t kVertexCount_ = 4;

	//アンカーポイント
	XMFLOAT2 anchorPoint_;

	//非表示フラグ
	bool isInvisible_ = false;
	
	//初期ポジション
	XMFLOAT2 position_;

	//頂点配列
	std::array<Vertex, kVertexCount_> vertices_{};

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	
	// 頂点データ全体のサイズ
	
	HRESULT result_;
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	//テクスチャマッピング用の変数
	//横方向ピクセル数
	const size_t kTextureWidth_ = 256;
	//縦方向ピクセル数
	const size_t kTextureHeight_ = 256;
	//配列の要素数
	const size_t imageDateCount_ = kTextureWidth_ * kTextureHeight_;
	//画像イメージデータ配列
	XMFLOAT4* imageDate_;
	//テクスチャリソースデスク
	D3D12_RESOURCE_DESC textureResourceDesc_{};

	D3D12_RESOURCE_DESC resDesc_{};

public:

	std::array <Vertex, kVertexCount_> GetterVertex() { return vertices_; };

	//ゲッター
	const XMFLOAT2& GetMoveSpeed_() const { return moveSpeed_; };
	float GetRotation() const { return rotation_; };
	const XMFLOAT4& GetColor() const { return color_; };
	const XMFLOAT2& GetSize() const { return size_; };
	const XMFLOAT2& GetAnchorPoint() const { return anchorPoint_; };
	const XMFLOAT2& GetPosition() const { return position_; };
	bool GetIsFlipX() const { return isFlipX_; };
	bool GetIsFlipY() const { return isFlipY_; };
	bool GetIsInvisible() const { return isInvisible_; };


	
	//セッター
	void SetMoveSpeed_(const DirectX::XMFLOAT2& moveSpeed) { moveSpeed_ = moveSpeed; };
	void SetRotation(float rotation) { rotation_ = rotation; };
	void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; };
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; };
	void SetAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetPosition(const DirectX::XMFLOAT2& initPosition) { position_ = initPosition; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	//ID3D12DescriptorHeap* GetSRVheap() { return srvHeap; };
	/*void SetWidth(size_t width) { wide_ = wide_; };
	void SetHeight(size_t height) { height_ = height; };*/
};
