#include "Camera.h"
#include <DirectXMath.h>
#include <d3d12.h>

DirectXBasic* Camera::directXBasic_ = nullptr;

using namespace DirectX;
using namespace MathUtility;

Camera::Camera() {};

void Camera::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Camera::Initialize(Vector3 eye, Vector3 target, Vector3 up)
{
	matView_.IdentityMatrix();
	matProjection_.IdentityMatrix();
	eye_	= eye;
	target_ = target;
	up_		= up;
}

void Camera::Updata()
{
	//ビュー行列の生成
	//matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	matView_ = Matrix4LookAtLH(eye_, target_, up_);

	//射影行列(透視投影)の生成
	matProjection_ =
		//Matrix4Perspective(
		//	fovAngleY,
		//	(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
		//	nearZ, farZ							//前端,奥端
		//);

		//Matrix4Perspective(
		//	fovAngleY,
		//	aspectRatio,	//アスペクト比(画面横幅/画面縦幅)
		//	nearZ, farZ							//前端,奥端
		//);

		//Matrix4Perspective(
		//	ToRadian(45.0f),//上下画角45度
		//	(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
		//	0.1f, 1000.0f							//前端,奥端
		//);


		//XMMatrixPerspectiveFovLH(
		//	XMConvertToRadians(45.0f),				//上下画角45度
		//	(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
		//	0.1f, 1000.0f							//前端,奥端
		//);
}
