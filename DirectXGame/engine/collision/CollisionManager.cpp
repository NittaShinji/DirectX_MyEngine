#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

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
		}
	}
}