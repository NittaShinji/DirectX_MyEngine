#include "Camera.h"
#include <cmath>
#include <cassert>

DirectXBasic* Camera::directXBasic_ = nullptr;

//using namespace DirectX;
using namespace MathUtillty;

void Camera::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Camera::Initialize()
{
	const Vector3 initcameraEye = { 30,7.5,-20 };
	const Vector3 initcameraTarget = { 0,5,5 };
	const Vector3 initcameraUp = { 0,1,0 };
	eye_ = initcameraEye;
	target_ = initcameraTarget;
	up_ = initcameraUp;
}

void Camera::Update()
{
	//ビュー行列の生成
	UpdateViewMatrix();

	//射影行列(透視投影)の生成
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	Vector3 vecEye = eye_;
	Vector3 vecTarget = target_;
	Vector3 vecUp = up_;

	//0ベクトルだと向きが定まらないので除外
	assert(!Vector3Equal(vecEye, vecTarget));
	//assert(std::isinf(cameraAxisZ));
	assert(!Vector3Equal(vecUp, Vector3Zero()));
	//assert(std::isinf(vecUp));

	//カメラZ軸(視線方向)
	Vector3 cameraAxisZ = Vector3Subtract(vecTarget, vecEye);

	//ベクトルを正規化
	cameraAxisZ.Normalize();

	//カメラのX軸(右方向)
	Vector3 cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = Vector3Cross(vecUp, cameraAxisZ);

	//ベクトルを正規化
	cameraAxisX.Normalize();

	//カメラのY軸(上方向)
	Vector3 cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = Vector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転行列
	Matrix4 matCameraRot = { 0 };
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;
	matCameraRot.m[0][3] = 0.0f;

	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;
	matCameraRot.m[1][3] = 0.0f;

	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;
	matCameraRot.m[2][3] = 0.0f;

	matCameraRot.m[3][0] = 0.0f;
	matCameraRot.m[3][1] = 0.0f;
	matCameraRot.m[3][2] = 0.0f;
	matCameraRot.m[3][3] = 1.0f;

	//転地により逆行列(逆回転)を計算
	matView_ = MatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	Vector3 reverseEyePosition = vecEye.Negate();

	//カメラの位置からワールド座標へのベクトル(カメラ座標系)
	Vector3 tX = Vector3Dot(cameraAxisX, reverseEyePosition); //X成分
	Vector3 tY = Vector3Dot(cameraAxisY, reverseEyePosition); //Y成分
	Vector3 tZ = Vector3Dot(cameraAxisZ, reverseEyePosition);//Z成分

	//一つのベクトルにまとめる
	Vector3 translation = { tX.x, tY.y, tZ.z };
	//ビュー行列に平行移動成分を指定
	matView_.m[3][0] = translation.x;
	matView_.m[3][1] = translation.y;
	matView_.m[3][2] = translation.z;
	matView_.m[3][3] = 1.0f;

#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard = MatrixIdentity();
	matBillboardY = MatrixIdentity();

	matBillboard.m[0][0] = cameraAxisX.x;
	matBillboard.m[0][1] = cameraAxisX.y;
	matBillboard.m[0][2] = cameraAxisX.z;

	matBillboard.m[1][0] = cameraAxisY.x;
	matBillboard.m[1][1] = cameraAxisY.y;
	matBillboard.m[1][2] = cameraAxisY.z;

	matBillboard.m[2][0] = cameraAxisZ.x;
	matBillboard.m[2][1] = cameraAxisZ.y;
	matBillboard.m[2][2] = cameraAxisZ.z;

	matBillboard.m[3][0] = 0;
	matBillboard.m[3][1] = 0;
	matBillboard.m[3][2] = 0;
	matBillboard.m[3][3] = 1;

#pragma endregion

#pragma region Y軸周りビルボード行列の計算
	//カメラX,Y,Z軸
	Vector3 ybillCameraAxisX;
	Vector3 ybillCameraAxisY;
	Vector3 ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = Vector3Normalize(vecUp);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = Vector3Cross(cameraAxisX, cameraAxisZ);

	//Y軸周りビルボード行列
	matBillboardY.m[0][0] = ybillCameraAxisX.x;
	matBillboardY.m[0][1] = ybillCameraAxisX.y;
	matBillboardY.m[0][2] = ybillCameraAxisX.z;

	matBillboardY.m[1][0] = ybillCameraAxisY.x;
	matBillboardY.m[1][1] = ybillCameraAxisY.y;
	matBillboardY.m[1][2] = ybillCameraAxisY.z;

	matBillboardY.m[2][0] = ybillCameraAxisZ.x;
	matBillboardY.m[2][1] = ybillCameraAxisZ.y;
	matBillboardY.m[2][2] = ybillCameraAxisZ.z;

	matBillboardY.m[3][0] = 0;
	matBillboardY.m[3][1] = 0;
	matBillboardY.m[3][2] = 0;
	matBillboardY.m[3][3] = 1;
#pragma endregion

}

void Camera::UpdateProjectionMatrix()
{
	matProjection_ = MatrixPerspectiveFovLH(ToRadian(45.0f),					//上下画角45度
		(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 1100.0f															//前端,奥端
	);
}
