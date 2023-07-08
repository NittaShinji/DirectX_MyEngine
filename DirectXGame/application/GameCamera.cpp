#include "GameCamera.h"

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
