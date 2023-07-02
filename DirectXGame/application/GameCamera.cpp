#include "GameCamera.h"

//void GameCamera::StaticInitialize(DirectXBasic* directXBasic){}
//
//void GameCamera::Initialize(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up){}
//
//void GameCamera::UpdateViewMatrix(){}
//
//void GameCamera::UpdateProjectionMatrix(){}

void GameCamera::Update(bool isPlayerMoving)
{
	if(isPlayerMoving == true)
	{
		target_.z += 0.25f;
		eye_.z += 0.25f;
	}

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void GameCamera::Reset()
{
	eye_ = { 30,7.5,-20 };
	target_ = { 0,5,5 };
}
