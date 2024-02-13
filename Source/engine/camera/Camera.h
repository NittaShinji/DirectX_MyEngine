#pragma once

#include "Input.h"
#include "DirectXBasic.h"
#include "MathUtillity.h"

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:

	//静的初期化
	static void StaticInitialize(DirectXBasic* directXBasic);

	//初期化
	virtual void Initialize();

	//更新
	void Update();

	//ビュー変換行列の更新
	virtual void UpdateViewMatrix();

	//プロジェクション行列の更新
	virtual void UpdateProjectionMatrix();

protected:

	static DirectXBasic* directXBasic_;

	//ビュー変換行列
	Matrix4 matView_;

	//ビュー行列の設定項目
	Vector3 eye_;		//視点座標
	Vector3 target_;	//注視点座標
	Vector3 up_;		//上方向ベクトル

	//カメラの回転角
	const float kDefaultAngle_ = 0.0f;
	float angle_ = kDefaultAngle_;

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
	const Matrix4& GetMatBillboardY() const { return matBillboardY; }

};

