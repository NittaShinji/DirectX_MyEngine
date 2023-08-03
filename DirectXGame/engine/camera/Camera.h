#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <wrl.h>
#include "Vector3.h"

class Camera
{
private:

	//DirectXを省略
	//using XMFLOAT = DirectX::Vector3;
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	static void StaticInitialize(DirectXBasic* directXBasic);

	virtual void Initialize(Vector3& eye, Vector3& target,Vector3& up);

	void Update();

	virtual void UpdateViewMatrix();

	virtual void UpdateProjectionMatrix();

protected:

	static DirectXBasic* directXBasic_;

	//ビュー変換行列
	XMMATRIX matView_;
	//ビュー行列の設定項目
	Vector3 eye_;		//視点座標
	Vector3 target_;	//注視点座標
	Vector3 up_;		//上方向ベクトル
	
	float angle_ = 0.0f;	//カメラの回転角

	//射影行列
	XMMATRIX matProjection_;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

public:

	//ゲッター
	const XMMATRIX& GetMatView() const { return matView_; }
	const XMMATRIX& GetMatProjection() const { return matProjection_; }
	const Vector3& GetEye() const { return eye_; }

};

