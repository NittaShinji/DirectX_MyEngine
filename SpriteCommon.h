#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "DirectXBasic.h"
using namespace DirectX;

class SpriteCommon
{
public:
	
	//初期化
	void Initialize(DirectXBasic* directXBasic);

	//ゲッター
	DirectXBasic* GetDirectXBasic(){ return directXBasic_; };

private:

	DirectXBasic* directXBasic_ = nullptr;

	//// 頂点データ全体のサイズ
	//UINT sizeVB;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
};