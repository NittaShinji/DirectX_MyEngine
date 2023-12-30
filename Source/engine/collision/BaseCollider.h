#pragma once
#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

/// <summary>
/// 基礎コライダー
/// </summary>
class BaseCollider
{
public:
	//デフォルトコンストラクタ
	BaseCollider() = default;
	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	friend class CollisionManager;

	//オブジェクトをメンバ変数のオブジェクトに代入
	inline void SetObject(Object3d* objcet)
	{
		this->objcet3d_ = objcet;
	}

	//メンバ変数のオブジェクトの値を返す
	inline Object3d* GetObject3d()
	{
		return objcet3d_;
	}

	//属性をメンバ変数の属性に代入
	inline void SetAttribute(unsigned short attribute)
	{
		attribute_ = attribute;
	}
	
	//メンバ変数に属性を追加
	inline void AddAttribute(unsigned short attribute) 
	{
		attribute_ |= attribute;
	}

	//メンバ変数の属性を削除
	inline void RemoveAttribute(unsigned short attribute)
	{
		attribute_ &= !attribute;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	//衝突時呼び出し関数
	inline void OnCollison(const CollisionInfo& info)
	{
		objcet3d_->OnCollision(info);
	}

protected:

	//オブジェクト
	Object3d* objcet3d_ = nullptr;
	//形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
	//当たり判定属性
	unsigned short attribute_ = 0b1111111111111111;
};