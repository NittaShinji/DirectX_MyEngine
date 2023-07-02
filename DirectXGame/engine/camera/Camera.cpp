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
	//�r���[�s��̐���
	//matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	UpdateViewMatrix();

	//�ˉe�s��(�������e)�̐���
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR vecEye_ = XMLoadFloat3(&eye_);		//���_���W
	XMVECTOR vecTarget_ = XMLoadFloat3(&target_);	//�����_���W
	XMVECTOR vecUp_ = XMLoadFloat3(&up_);		//������x�N�g��

	//�J����Z��(��������)
	XMVECTOR cameraAxisZ = XMVectorSubtract(vecTarget_, vecEye_);

	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(vecUp_, XMVectorZero()));
	assert(!XMVector3IsInfinite(vecUp_));

	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X��(�E����)
	XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(vecUp_, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y��(�����)
	XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	XMMATRIX matCameraRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//�]�n�ɂ��t�s��(�t��])���v�Z
	matView_ = XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(vecEye_);
	//�J�����̈ʒu���烏�[���h���W�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition); //X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition); //Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition); //Z����
	//��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	//�r���[�s��ɕ��s�ړ��������w��
	matView_.r[3] = translation;

}

void Camera::UpdateProjectionMatrix()
{
	matProjection_ =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//�㉺��p45�x
			(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
			0.1f, 1000.0f							//�O�[,���[
		);
}
