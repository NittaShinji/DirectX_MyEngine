#include "Camera.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <cmath>

DirectXBasic* Camera::directXBasic_ = nullptr;

using namespace DirectX;
using namespace MathUtillty;

void Camera::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Camera::Initialize(Vector3& eye, Vector3& target, Vector3& up)
{
	eye_	= eye;
	target_ = target;
	up_		= up;
}

void Camera::Update()
{
	//�r���[�s��̐���
	//matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	UpdateViewMatrix();

	//�ˉe�s��(�������e)�̐���
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	//XMVECTOR vecEye_ = XMLoadFloat3(&eye_);		//���_���W
	//XMVECTOR vecTarget_ = XMLoadFloat3(&target_);	//�����_���W
	//XMVECTOR vecUp_ = XMLoadFloat3(&up_);
	//
	Vector3 vecEye = eye_;
	Vector3 vecTarget = target_;
	Vector3 vecUp = up_;

	//������x�N�g��

	//�J����Z��(��������)
	//XMVECTOR cameraAxisZ = XMVectorSubtract(vecTarget_, vecEye_);
	//Vector3 cameraAxisZ = vecTarget_ -  vecEye_);
	Vector3 cameraAxisZ = Vector3Subtract(vecTarget, vecEye);

	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	//assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	//assert(!XMVector3IsInfinite(cameraAxisZ));
	//assert(!XMVector3Equal(vecUp_, XMVectorZero()));
	//assert(!XMVector3IsInfinite(vecUp_));
	/*assert(!Vector3Equal(cameraAxisZ, Vector3Zero()));
	assert(std::isinf(cameraAxisZ));
	assert(!Vector3Equal(vecUp, Vector3Zero()));
	assert(std::isinf(vecUp));*/

	//�x�N�g���𐳋K��
	//cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	cameraAxisZ.Normalize();

	//�J������X��(�E����)
	Vector3 cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	//cameraAxisX = XMVector3Cross(vecUp_, cameraAxisZ);
	cameraAxisX = Vector3Cross(vecUp, cameraAxisZ);
	
	//�x�N�g���𐳋K��
	//cameraAxisX = XMVector3Normalize(cameraAxisX);
	cameraAxisX.Normalize();

	//�J������Y��(�����)
	Vector3 cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	//cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = Vector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	//XMMATRIX matCameraRot;
	Matrix4 matCameraRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;
	matCameraRot.m[0][3] = 0;

	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;
	matCameraRot.m[1][3] = 0;

	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;
	matCameraRot.m[2][3] = 0;

	matCameraRot.m[3][0] = 0;
	matCameraRot.m[3][1] = 0;
	matCameraRot.m[3][2] = 0;
	matCameraRot.m[3][3] = 1;

	//�]�n�ɂ��t�s��(�t��])���v�Z
	//matView_ = XMMatrixTranspose(matCameraRot);
	matView_ = MatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	//XMVECTOR reverseEyePosition = XMVectorNegate(vecEye_);
	Vector3 reverseEyePosition = vecEye.Negate();

	//�J�����̈ʒu���烏�[���h���W�ւ̃x�N�g��(�J�������W�n)
	//XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition); //X����
	//XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition); //Y����
	//XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition); //Z����

	Vector3 tX = Vector3Dot(cameraAxisX, reverseEyePosition); //X����
	Vector3 tY = Vector3Dot(cameraAxisY, reverseEyePosition); //Y����
	Vector3 tZ = Vector3Dot(cameraAxisZ, reverseEyePosition);//Z����

	//��̃x�N�g���ɂ܂Ƃ߂�
	//XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	Vector3 translation = { tX.x, tY.y, tZ.z};
	//�r���[�s��ɕ��s�ړ��������w��
	matView_.m[3][0] = translation.x;
	matView_.m[3][1] = translation.y;
	matView_.m[3][2] = translation.z;
	matView_.m[3][3] = 1.0f;
}

void Camera::UpdateProjectionMatrix()
{
	//matProjection_ =
	//	XMMatrixPerspectiveFovLH(
	//		XMConvertToRadians(45.0f),				//�㉺��p45�x
	//		(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
	//		0.1f, 1100.0f							//�O�[,���[
	//	);

	matProjection_ = MatrixPerspectiveFovLH(ToRadian(45.0f),
		(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),
		0.1f, 1100.0f
	);
}
