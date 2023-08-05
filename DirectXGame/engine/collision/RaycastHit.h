#pragma once
#include "BaseCollider.h"
#include <DirectXMath.h>

class  Object3d;

/// <summary>
/// ���C�L���X�g�ɂ����𓾂邽�߂̍\����
/// </summary>
struct RaycastHit
{
	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	//DirectX::XMVECTOR inter;
	Vector3 inter;
	//�Փ˓_�܂ł̋���
	float distance = 0.0f;
};