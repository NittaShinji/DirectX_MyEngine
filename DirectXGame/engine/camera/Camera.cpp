#include "Camera.h"
#include <DirectXMath.h>
#include <d3d12.h>

DirectXBasic* Camera::directXBasic_ = nullptr;

using namespace DirectX;

void Camera::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Camera::Initialize(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up)
{
	eye_	= eye;
	target_ = target;
	up_		= up;
}

void Camera::Update()
{
	//ビュー行列の生成
	//matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	UpdateViewMatrix();

	//射影行列(透視投影)の生成
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR vecEye_ = XMLoadFloat3(&eye_);		//視点座標
	XMVECTOR vecTarget_ = XMLoadFloat3(&target_);	//注視点座標
	XMVECTOR vecUp_ = XMLoadFloat3(&up_);		//上方向ベクトル

	//カメラZ軸(視線方向)
	XMVECTOR cameraAxisZ = XMVectorSubtract(vecTarget_, vecEye_);

	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(vecUp_, XMVectorZero()));
	assert(!XMVector3IsInfinite(vecUp_));

	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(vecUp_, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸(上方向)
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//転地により逆行列(逆回転)を計算
	matView_ = XMMatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(vecEye_);
	//カメラの位置からワールド座標へのベクトル(カメラ座標系)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition); //X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition); //Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition); //Z成分
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	//ビュー行列に平行移動成分を指定
	matView_.r[3] = translation;

}

void Camera::UpdateProjectionMatrix()
{
	matProjection_ =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//上下画角45度
			(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
			0.1f, 1000.0f							//前端,奥端
		);
}
