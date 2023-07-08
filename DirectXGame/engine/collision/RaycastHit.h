#pragma once
#include "BaseCollider.h"
#include <DirectXMath.h>

class  Objcect3d;

/// <summary>
/// レイキャストによる情報を得るための構造体
/// </summary>
struct RaycastHit
{
	//衝突相手のオブジェクト
	Objcect3d* object = nullptr;
};