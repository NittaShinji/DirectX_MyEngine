#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
//#include <DirectXMath.h>
#include <wrl.h>
#include "MathUtillity.h"

class Camera
{
public:

	static void StaticInitialize(DirectXBasic* directXBasic);

	virtual void Initialize(Vector3& eye, Vector3& target, Vector3& up);

	void Update();

	virtual void UpdateViewMatrix();

	virtual void UpdateProjectionMatrix();

protected:

	static DirectXBasic* directXBasic_;

	//ビュー変換行列
	Matrix4 matView_;

	//ビュー行列の設定項目
	Vector3 eye_;		//視点座標
	Vector3 target_;	//注視点座標
	Vector3 up_;		//上方向ベクトル

	float angle_ = 0.0f;	//カメラの回転角

	//射影行列
	Matrix4 matProjection_;

	//ビルボード行列
	Matrix4 matBillboard;
	//Y軸ビルボード行列
	Matrix4 matBillboardY;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

public:

	//ゲッター
	const Matrix4& GetMatView() const { return matView_; }
	const Matrix4& GetMatProjection() const { return matProjection_; }
	const Vector3& GetEye() const { return eye_; }
	const Matrix4& GetMatBillboard() const { return matBillboard; }
};

