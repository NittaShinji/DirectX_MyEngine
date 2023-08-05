#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: //�G�C���A�X

	//using XMVECTOR = DirectX::XMVECTOR;

public:
	//SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
	SphereCollider(Vector3 offset = { 0,0,0 }, float radius = 1.0f) :

		offset_(offset),
		radius_(radius)
	{
		//���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline void SetRadius(float radius) { this->radius = radius; }

	//inline const XMVECTOR& GetOffset() { return offset; }
	inline const Vector3& GetOffset() { return offset_; }

	//inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }
	inline void SetOffset(const Vector3& offset) { this->offset_ = offset; }


	inline float GetRadius() { return radius; }

private:
	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	//XMVECTOR offset;
	Vector3 offset_;
	//���a
	float radius_;
};
