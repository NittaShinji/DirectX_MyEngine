#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include "Vector2.h"

class GameCamera : public Camera
{
public:

	void Update(bool isPlayerMoving);

	void Reset();

	void ImGuiUpdate();
};

