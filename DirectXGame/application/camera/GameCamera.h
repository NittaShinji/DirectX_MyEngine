#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include "Vector2.h"


class GameCamera : public Camera
{

	//エイリアステンプレート
	//using XMFLOAT = DirectX::Vector3;
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	void Update(bool isPlayerMoving);

	void Reset();
};

