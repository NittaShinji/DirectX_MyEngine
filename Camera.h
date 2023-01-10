#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include "MathUtility.h"
#include "Matrix4.h"

#include <wrl.h>

class Camera
{
	//エイリアステンプレート
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	Camera();

	static void StaticInitialize(DirectXBasic* directXBasic);

	void Initialize(Vector3 eye, Vector3 target, Vector3 up);

	void Updata();

private:

	static DirectXBasic* directXBasic_;

	//ビュー変換行列
	Matrix4 matView_;
	//ビュー行列の設定項目
	Vector3 eye_;		//視点座標
	Vector3 target_;	//注視点座標
	Vector3 up_;		//上方向ベクトル
	
	float angle_ = 0.0f;	//カメラの回転角

	// 垂直方向視野角
	float fovAngleY = 45.0f * MathUtility::PI / 180.0f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;

	//射影行列
	Matrix4 matProjection_;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	//ConstBufferDateTransform* constMapTransform

public:

	//ゲッター
	const Matrix4 GetMatView() const { return matView_; };
	const Matrix4 GetMatProjection() const { return matProjection_; };

};

