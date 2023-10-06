#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;
using namespace MathUtillty;

void MeshCollider::ConstructTriangles(Model* model)
{
	//三角形リストをクリア
	triangles.clear();
	//モデルの持つメッシュリストを取得
	std::vector<Mesh> meshes = model->GetMeshes();
	//現在のメッシュの開始三角形番号を入れておく変数(0で初期化)
	int start = 0;
	//全メッシュについて順に処理する
	std::vector<Mesh>::iterator it = meshes.begin();

	for(; it != meshes.cend(); ++it)
	{
		const std::vector<Mesh::Vertex>& vertices = it->GetVertices();
		const std::vector<unsigned short>& indices = it->GetIndices();

		//インデックスは、三角形の数 x 3個あるので
		//そこからメッシュ内の三角形を逆算する
		size_t triangleNum = indices.size() / 3;
		//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
		triangles.resize(triangles.size() + triangleNum);
		//全三角形について順に処理する
		for(int i = 0; i < triangleNum; i++)
		{
			//今から計算する三角形の参照
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];
			//三角形の3頂点の座標を大入
			tri.p0_ = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, };
			tri.p1_ = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, };
			tri.p2_ = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, };
			//3頂点から法線を計算
			tri.ComputeNormal();
		}

		//次のメッシュは、今までの三角形番号の次から使う
		start += (int)triangleNum;
	}

}

void MeshCollider::Update()
{
	//ワールド行列の逆行列を計算
	invMatWorld_ = MatrixInverse(GetObject3d()->GetMatWorld());

}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter)

{
	//オブジェクトのローカル座標系での球を得る(半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = Vector3Transform(sphere.center, invMatWorld_)
		;
	localSphere.radius *= invMatWorld_.m[0][0];


	//ローカル座標系で交差をチェック
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for(; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		//球と三角形の当たり判定
		if(Collision::CheckSphere2Triangle(localSphere, triangle, inter))
		{
			if(inter)
			{
				const Matrix4& matWorld = GetObject3d()->GetMatWorld();

				//ワールド座標系での交点を得る
				*inter = Vector3Transform(*inter, matWorld);

			}
			return true;
		}
	}



	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, Vector3* inter)

{
	//オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = Vector3Transform(ray.start, invMatWorld_);
	localRay.dir = Vector3TransformNormal(ray.dir, invMatWorld_);
	localRay.dir = localRay.dir.Normalize();

	//ローカル座標系で交差をチェック
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for(; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;

		Vector3 tempInter;

		//レイと三角形の当たり判定
		if(Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter))
		{
			const Matrix4& matWorld = GetObject3d()->GetMatWorld();

			//ワールド座標系での交点を得る
			tempInter = Vector3Transform(tempInter, matWorld);

			if(distance)
			{
				//交点とレイ始点の距離を計算
				Vector3 sub = tempInter - ray.start;
				*distance = Vector3Dot(sub, ray.dir).x;
			}

			if(inter)
			{
				*inter = tempInter;
			}

			return true;
		}

	}

	return false;
}
