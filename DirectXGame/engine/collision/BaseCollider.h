#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

class BaseCollider
{
public:
	BaseCollider() = default;
	//仮想デストラクタ
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
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	inline void OnCollison(const CollisionInfo& info)
	{
		objcet3d_->OnCollision(info);
	}

protected:

	Object3d* objcet3d_ = nullptr;
	//形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	//当たり判定属性
	unsigned short attribute = 0b1111111111111111;

};