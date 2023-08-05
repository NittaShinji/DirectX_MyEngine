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
	UpdateViewMatrix();

	//�ˉe�s��(�������e)�̐���
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	Vector3 vecEye = eye_;
	Vector3 vecTarget = target_;
	Vector3 vecUp = up_;

	//������x�N�g��

	//�J����Z��(��������)
	Vector3 cameraAxisZ = Vector3Subtract(vecTarget, vecEye);

	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!Vector3Equal(cameraAxisZ, Vector3Zero()));
	//assert(std::isinf(cameraAxisZ));
	assert(!Vector3Equal(vecUp, Vector3Zero()));
	//assert(std::isinf(vecUp));

	//�x�N�g���𐳋K��
	cameraAxisZ.Normalize();

	//�J������X��(�E����)
	Vector3 cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = Vector3Cross(vecUp, cameraAxisZ);
	
	//�x�N�g���𐳋K��
	cameraAxisX.Normalize();

	//�J������Y��(�����)
	Vector3 cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = Vector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	Matrix4 matCameraRot = {0};
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
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

	//�]�n�ɂ��t�s��(�t��])���v�Z
	matView_ = MatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	Vector3 reverseEyePosition = vecEye.Negate();

	//�J�����̈ʒu���烏�[���h���W�ւ̃x�N�g��(�J�������W�n)
	Vector3 tX = Vector3Dot(cameraAxisX, reverseEyePosition); //X����
	Vector3 tY = Vector3Dot(cameraAxisY, reverseEyePosition); //Y����
	Vector3 tZ = Vector3Dot(cameraAxisZ, reverseEyePosition);//Z����

	//��̃x�N�g���ɂ܂Ƃ߂�
	Vector3 translation = { tX.x, tY.y, tZ.z};
	//�r���[�s��ɕ��s�ړ��������w��
	matView_.m[3][0] = translation.x;
	matView_.m[3][1] = translation.y;
	matView_.m[3][2] = translation.z;
	matView_.m[3][3] = 1.0f;
}

void Camera::UpdateProjectionMatrix()
{
	matProjection_ = MatrixPerspectiveFovLH(ToRadian(45.0f),					//�㉺��p45�x
		(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1100.0f															//�O�[,���[
	);
}
