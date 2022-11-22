#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "SpriteCommon.h"
using namespace DirectX;

//スプライト
class Sprite
{
public:

	//初期化
	void Initialize(SpriteCommon* spriteCommon);

private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;

	struct Vertex
	{
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//画像内のxyz座標
	};

	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	// 頂点データ全体のサイズ
	UINT sizeVB;

};
