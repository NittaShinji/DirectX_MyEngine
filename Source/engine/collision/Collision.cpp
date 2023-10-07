#include "Collision.h"
#include <cmath>

using namespace MathUtillty;

Collision::Collision() {}
Collision::~Collision() {}

//球と平面の当たり判定
bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	//座標系の原点から球の中心座標への距離
	Vector3 distV = Vector3Dot(sphere.center, plane.normal);

	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.x - plane.distance;

	//距離の絶対値が半径より大きければ当たってない
	if(fabsf(dist) > sphere.radius)
	{
		return false;
	}

	//擬似交点を計算
	if(inter)
	{
		//平面上の最大接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
};

void Collision::ClosetPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	Vector3 p0_p2 = triangle.p2_ - triangle.p0_;
	Vector3 p0_pt = point - triangle.p0_;

	Vector3 d1 = Vector3Dot(p0_p1, p0_pt);
	Vector3 d2 = Vector3Dot(p0_p2, p0_pt);

	if(d1.x <= 0.0f && d2.x <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0_;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector3 p1_pt = point - triangle.p1_;
	Vector3 d3 = Vector3Dot(p0_p1, p1_pt);
	Vector3 d4 = Vector3Dot(p0_p2, p1_pt);

	if(d3.x >= 0.0f && d4.x <= d3.x)
	{
		// p1が最近傍
		*closest = triangle.p1_;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.x * d4.x - d3.x * d2.x;

	if(vc <= 0.0f && d1.x >= 0.0f && d3.x <= 0.0f)
	{
		float v = d1.x / (d1.x - d3.x);
		*closest = triangle.p0_ + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3 p2_pt = point - triangle.p2_;
	Vector3 d5 = Vector3Dot(p0_p1, p2_pt);
	Vector3 d6 = Vector3Dot(p0_p2, p2_pt);

	if(d6.x >= 0.0f && d5.x <= d6.x)
	{
		*closest = triangle.p2_;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.x * d2.x - d1.x * d6.x;
	if(vb <= 0.0f && d2.x >= 0.0f && d6.x <= 0.0f)
	{
		float w = d2.x / (d2.x - d6.x);
		*closest = triangle.p0_ + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.x * d6.x - d5.x * d4.x;
	if(va <= 0.0f && (d4.x - d3.x) >= 0.0f && (d5.x - d6.x) >= 0.0f)
	{
		float w = (d4.x - d3.x) / ((d4.x - d3.x) + (d5.x - d6.x));
		*closest = triangle.p1_ + w * (triangle.p2_ - triangle.p1_);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0_ + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;

	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosetPtPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	Vector3 v = p - sphere.center;

	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = Vector3Dot(v, v);

	//球と三角形の距離が半径以下なら当たっていない
	if(v.x > sphere.radius * sphere.radius)
	{
		return false;
	}

	//疑似交点を計算
	if(inter)
	{
		//三角形上の最近接点pを疑似交点とする
		*inter = p;
	}

	return true;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	//球と球の距離
	float distance = (sphereB.pos.x - sphereA.pos.x) * (sphereB.pos.x - sphereA.pos.x) +
		(sphereB.pos.y - sphereA.pos.y) * (sphereB.pos.y - sphereA.pos.y) +
		(sphereB.pos.z - sphereA.pos.z) * (sphereB.pos.z - sphereA.pos.z);

	//半径と半径を足したもの
	float addRadius = (sphereA.radius + sphereB.radius) * (sphereA.radius + sphereB.radius);

	//衝突(半径内に入っているかどうか)
	if(distance <= addRadius)
	{
		return true;
	}

	return false;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)

{
	const float epsilon = 1.0e-5f; //誤差吸収用の微小な値
	//面法線とレイの方向ベクトルの内積
	float d1 = Vector3Dot(plane.normal, ray.dir).x;
	//裏面には当たらない
	if(d1 > -epsilon) { return false; }
	//始点と原点の距離(平面の法線方向)
	//面法線とレイの視点座標(位置ベクトル)の内積
	float d2 = Vector3Dot(plane.normal, ray.start).x;
	//始点と平面の距離(平面の法線方向)
	float dist = d2 - plane.distance;
	//始点と平面の距離(レイ方向)
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if(t < 0) return false;
	//距離を書き込む
	if(distance) { *distance = t; }
	//交点を計算
	if(inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)

{
	//三角形が乗っている平面を算出
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal_;
	plane.distance = Vector3Dot(triangle.normal_, triangle.p0_).x;
	//レイと平面が当たっていなければ、当たっていない
	if(!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; //誤差吸収用の微小な値

	Vector3 m;
	//辺p0_p1について
	Vector3 pt_p0 = triangle.p0_ - interPlane;
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	m = Vector3Cross(pt_p0, p0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if(Vector3Dot(m, triangle.normal_).x < -epsilon) { return false; }

	//辺p1,p2について
	Vector3 pt_p1 = triangle.p1_ - interPlane;
	Vector3 p1_p2 = triangle.p2_ - triangle.p1_;
	m = Vector3Cross(pt_p1, p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if(Vector3Dot(m, triangle.normal_).x < -epsilon) { return false; }

	//辺p2,p0について
	Vector3 pt_p2 = triangle.p2_ - interPlane;
	Vector3 p2_p0 = triangle.p0_ - triangle.p2_;
	m = Vector3Cross(pt_p2, p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if(Vector3Dot(m, triangle.normal_).x < -epsilon) { return false; }

	//内側なので当たっている
	if(inter)
	{
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)

{
	Vector3 m = ray.start - sphere.center;
	float b = Vector3Dot(m, ray.dir).x;
	float c = Vector3Dot(m, m).x - sphere.radius * sphere.radius;

	//layの始点がsphereの外側にあり(c > 0)、layがsphereから離れていく方向を差している場合(b > 0)、当たらない
	if(c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	//負の判別式はレイが球を外れていることに一致
	if(discr < 0.0f) { return false; }

	//レイは球と交差している
	//交差する最小の値tを計算
	float t = b - sqrtf(discr);
	//tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if(t < 0) t = 0.0f;
	if(distance) { *distance = t; }
	if(inter) { *inter = ray.start + t * ray.dir; }

	return true;
}
