#pragma once

#include <DirectXMath.h>

class Object3d;
class BaseCollider;

/// <summary>
/// �Փˏ��
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(Object3d* object,BaseCollider* collider, const Vector3& inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	Vector3 inter;
};