#include "Camera.h"
#include <DirectXMath.h>
#include <d3d12.h>

DirectXBasic* Camera::directXBasic_ = nullptr;

using namespace DirectX;

void Camera::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
}

void Camera::Initialize(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	eye_	= eye;
	target_ = target;
	up_		= up;
}

void Camera::Updata()
{
	//�r���[�s��̐���
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));

	//�ˉe�s��(�������e)�̐���
	matProjection_ =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//�㉺��p45�x
			(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
			0.1f, 1000.0f							//�O�[,���[
		);
}
