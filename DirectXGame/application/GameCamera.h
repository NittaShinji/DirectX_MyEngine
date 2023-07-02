#pragma once
#include "Camera.h"
#include <DirectXMath.h>

class GameCamera : public Camera
{

	//エイリアステンプレート
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	//static void StaticInitialize(DirectXBasic* directXBasic);

	//void Initialize(XMFLOAT3& eye, XMFLOAT3& target, XMFLOAT3& up) override;

	void Update(bool isPlayerMoving);

	void Reset();

	//void UpdateViewMatrix() override;

	//void UpdateProjectionMatrix() override;

};

