#pragma once
#include "BaseCollider.h"
#include <DirectXMath.h>

class  Objcect3d;

/// <summary>
/// ���C�L���X�g�ɂ����𓾂邽�߂̍\����
/// </summary>
struct RaycastHit
{
	//�Փˑ���̃I�u�W�F�N�g
	Objcect3d* object = nullptr;
};