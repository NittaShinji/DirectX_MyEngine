#pragma once
#include "Camera.h"
#include "Vector2.h"

class GameCamera : public Camera
{
public:

	void Update(bool isPlayerMoving, Vector3 playerAxcell_, Vector3 pInitPos);

	void Reset();

	void ImGuiUpdate();
};

