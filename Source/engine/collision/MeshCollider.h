#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "Matrix4.h"

/// <summary>
/// メッシュ衝突判定オブジェクト
/// </summary>
class MeshCollider : public NsEngine::BaseCollider
{

public:

	//コンストラクタ
	MeshCollider()
	{
		//メッシュ形状をセット
		shapeType_ = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	/// <param name="model"></param>
	void ConstructTriangles(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差している否か</returns>
	bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);

	/// <summary>
	/// レイとの当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);

private:

	//メッシュが持つ三角形の配列
	std::vector<Triangle> triangles;
	//ワールド行列の逆行列
	Matrix4 invMatWorld_;
};

