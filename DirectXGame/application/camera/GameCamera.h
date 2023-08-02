#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include "Vector2.h"


class GameCamera : public Camera
{

	//�G�C���A�X�e���v���[�g
	using XMFLOAT = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	void Update(bool isPlayerMoving);

	void Reset();
};

