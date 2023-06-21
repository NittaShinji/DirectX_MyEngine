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

void Camera::Updata()
{
	//ビュー行列の生成
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));

	//射影行列(透視投影)の生成
	matProjection_ =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				//上下画角45度
			(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//アスペクト比(画面横幅/画面縦幅)
			0.1f, 1000.0f							//前端,奥端
		);
}
