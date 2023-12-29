#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider : public BaseCollider, public Sphere
{
public:

	static const float kDefaultRadius_;
	static const Vector3 kDefaultOffset_;

public:

	//コンストラクタ
	SphereCollider(Vector3 offset = kDefaultOffset_, float radius = kDefaultRadius_) :

		offset_(offset),
		radius_(radius)
	{
		//球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	//オブジェクト中心からのオフセット
	Vector3 offset_;
	//半径
	float radius_;

public:

	inline void SetRadius(float radius) { radius_ = radius; }
	inline void SetOffset(const Vector3& offset) { this->offset_ = offset; }
	inline const Vector3& GetOffset() { return offset_; }
	inline float GetRadius() { return sphereRadius; }

};
