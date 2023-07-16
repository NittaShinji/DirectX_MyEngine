#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

class BaseCollider
{
public:
	BaseCollider() = default;
	//���z�f�X�g���N�^
	virtual ~BaseCollider() = default;

	friend class CollisionManager;

	inline void SetObject(Object3d* objcet)
	{
		this->objcet3d_ = objcet;
	}

	inline Object3d* GetObject3d()
	{
		return objcet3d_;
	}

	inline void SetAttribute(unsigned short attribute)
	{
		this->attribute = attribute;
	}

	inline void AddAttribute(unsigned short attribute)
	{
		this->attribute |= attribute;
	}

	inline void RemoveAttribute(unsigned short attribute)
	{
		this->attribute &= !attribute;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	inline void OnCollison(const CollisionInfo& info)
	{
		objcet3d_->OnCollision(info);
	}

protected:

	Object3d* objcet3d_ = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	//�����蔻�葮��
	unsigned short attribute = 0b1111111111111111;

};