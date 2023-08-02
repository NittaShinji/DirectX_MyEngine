#pragma once
#include <DirectXMath.h>

class WorldTransform
{
	using XMFLOAT = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	XMMATRIX worldTransform;

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

};

