#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <wrl.h>

class Camera
{
private:

	//DirectXを省略
	using XMFLOAT = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	static void StaticInitialize(DirectXBasic* directXBasic);

	virtual void Initialize(XMFLOAT3& eye, XMFLOAT3& target,XMFLOAT3& up);

	void Update();

	virtual void UpdateViewMatrix();

	virtual void UpdateProjectionMatrix();

protected:

	static DirectXBasic* directXBasic_;

	//ビュー変換行列
	XMMATRIX matView_;
	//ビュー行列の設定項目
	XMFLOAT3 eye_;		//視点座標
	XMFLOAT3 target_;	//注視点座標
	XMFLOAT3 up_;		//上方向ベクトル
	
	float angle_ = 0.0f;	//カメラの回転角

	//射影行列
	XMMATRIX matProjection_;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

public:

	//ゲッター
	const XMMATRIX& GetMatView() const { return matView_; }
	const XMMATRIX& GetMatProjection() const { return matProjection_; }
	const XMFLOAT3& GetEye() const { return eye_; }

};

