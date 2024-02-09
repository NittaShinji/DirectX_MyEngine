#pragma once

class Object3d;

namespace NsEngine
{
	class BaseCollider;
}

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(Object3d* object, NsEngine::BaseCollider* collider, const Vector3& inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	//衝突相手のオブジェクト
	Object3d* object = nullptr;
	//衝突相手のコライダー
	NsEngine::BaseCollider* collider = nullptr;
	//衝突点
	Vector3 inter;
};