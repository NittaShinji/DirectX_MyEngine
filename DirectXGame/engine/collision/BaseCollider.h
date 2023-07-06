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

	inline void SetObject(Object3d* objcet)
	{
		this->objcet3d_ = objcet;
	}

	inline Object3d* GetObject3d()
	{
		return objcet3d_;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	inline void OnCollison(const CollisionInfo& info) 
	{
		objcet3d_->OnCollison(info);
	}

protected:

	Object3d* objcet3d_ = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
};