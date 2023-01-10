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
	//�r���[�s��̐���
	//matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	matView_ = Matrix4LookAtLH(eye_, target_, up_);

	//�ˉe�s��(�������e)�̐���
	matProjection_ =
		//Matrix4Perspective(
		//	fovAngleY,
		//	(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		//	nearZ, farZ							//�O�[,���[
		//);

		//Matrix4Perspective(
		//	fovAngleY,
		//	aspectRatio,	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		//	nearZ, farZ							//�O�[,���[
		//);

		//Matrix4Perspective(
		//	ToRadian(45.0f),//�㉺��p45�x
		//	(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		//	0.1f, 1000.0f							//�O�[,���[
		//);


		//XMMatrixPerspectiveFovLH(
		//	XMConvertToRadians(45.0f),				//�㉺��p45�x
		//	(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		//	0.1f, 1000.0f							//�O�[,���[
		//);
}
