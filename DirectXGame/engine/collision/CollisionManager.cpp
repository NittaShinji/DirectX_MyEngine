#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	//全ての組み合わせについて総当たりチェック
	itA = colliders.begin();
	for(;itA != colliders.end();  ++itA)
	{
		itB = itA;
		++itB;
		for(;itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;

				if(Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
				{
					colA->OnCollison(CollisionInfo(colB->GetObject3d(),colB,inter));
					colB->OnCollison(CollisionInfo(colA->GetObject3d(),colA,inter));
				}
			}
			else if(colA->GetShapeType() == COLLISIONSHAPE_MESH && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if(meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					colA->OnCollison(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollison(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_MESH)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if(meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					colA->OnCollison(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollison(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
		}
	}
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	//bool result = false;
	////走査用のイテレータ
	//std::forward_list<BaseCollider*>::iterator it;
	////今まで最も近いコライダーを記録する為のイテレータ
	//std::forward_list<BaseCollider*>::iterator itHit;
	////今まで最も近いコライダーの距離を記録する変数
	//float distance = maxDistance;
	////今までで最も近いコライダーとの交点を記録する関数
	//XMVECTOR inter;

	////全てのコライダーと総当たりチェック
	//it = colliders.begin();
	//for(;it != colliders.end();  ++it)
	//{
	//	BaseCollider* colA = *it;
	//	//球の場合
	//	if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
	//	{
	//		Sphere* sphere = dynamic_cast<Sphere*>(colA);
	//		float tempDistance;
	//		XMVECTOR tempInter;
	//		//当たらなければ除外
	//		if(!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
	//		//距離が最小でなければ除外
	//		if(tempDistance >= distance) continue;
	//		//今まで最も近いので記録を取る
	//		result = true;
	//		distance = tempDistance;
	//		inter = tempInter;
	//		itHit = it;
	//	}
	//	else if(colA->GetShapeType() == COLLISIONSHAPE_MESH)
	//	{
	//		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
	//		float tempDistance;
	//		DirectX::XMVECTOR tempInter;
	//		if(!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
	//		if(tempDistance >= distance) continue;
	//		
	//		result = true;
	//		distance = tempDistance;
	//		inter = tempInter;
	//		itHit = it;
	//	}
	//}

	////最終的になにかに当たっていたら結果を書き込む
	//if(result && hitInfo)
	//{
	//	hitInfo->distance = distance;
	//	hitInfo->inter = inter;
	//	hitInfo->collider = *itHit;
	//	hitInfo->object = hitInfo->collider->GetObject3d();
	//}

	//return result;

	//全属性有効にして属性版を実行
	return Raycast(ray, 0xffff, hitInfo, maxDistance);

}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	//走査用のイテレータ
	std::forward_list<BaseCollider*>::iterator it;
	//今まで最も近いコライダーを記録する為のイテレータ
	std::forward_list<BaseCollider*>::iterator itHit;
	//今まで最も近いコライダーの距離を記録する変数
	float distance = maxDistance;
	//今までで最も近いコライダーとの交点を記録する関数
	XMVECTOR inter;

	//全てのコライダーと総当たりチェック
	it = colliders.begin();
	for(; it != colliders.end(); ++it)
	{
		BaseCollider* colA = *it;

		//属性が合わなければスキップ
		if(!(colA->attribute & attribute))
		{
			continue;
		}

		//球の場合
		if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			//当たらなければ除外
			if(!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			//距離が最小でなければ除外
			if(tempDistance >= distance) continue;
			//今まで最も近いので記録を取る
			result = true;
			distance = tempDistance;
			inter = tempInter;
			itHit = it;
		}
		else if(colA->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
			float tempDistance;
			DirectX::XMVECTOR tempInter;
			if(!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if(tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			itHit = it;
		}
	}

	//最終的になにかに当たっていたら結果を書き込む
	if(result && hitInfo)
	{
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *itHit;
		hitInfo->object = hitInfo->collider->GetObject3d();
	}

	return result;

}

