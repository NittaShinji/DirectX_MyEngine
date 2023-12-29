#pragma once
#include"MathUtillity.h"

class Object3d;
class BaseCollider;

/// <summary>
/// クエリ―による情報を得る為の構造体
/// </summary>
struct QueryHit
{
	//衝突相手のオブジェクト
	Object3d* object = nullptr;
	//衝突相手のオブジェクト
	BaseCollider* collider = nullptr;
	//衝突点
	Vector3 inter;
	//排斥ベクトル
	Vector3 reject;
};

/// <summary>
/// クエリ―で交差を検出した時の動作を規定するクラス
/// </summary>
class QueryCallback
{
public:

	//デフォルトコンストラクタ
	QueryCallback() = default;
	//デフォルトデストラクタ
	virtual ~QueryCallback() = default;

	/// <summary>
	/// 交差時コールバック
	/// </summary>
	/// <param name="info">交差情報</param>
	/// <returns>クエリ―を続けるならtrue、打ち切るならfalseを返す</returns>
	virtual bool OnQueryHit(const QueryHit& info) = 0;

};

