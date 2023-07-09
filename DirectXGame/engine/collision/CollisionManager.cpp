#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

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

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
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

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	//�����p�̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it;
	//���܂ōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator itHit;
	//���܂ōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = maxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����֐�
	XMVECTOR inter;

	//�S�ẴR���C�_�[�Ƒ�������`�F�b�N
	it = colliders.begin();
	for(;it != colliders.end();  ++it)
	{
		BaseCollider* colA = *it;
		//���̏ꍇ
		if(colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			//������Ȃ���Ώ��O
			if(!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			//�������ŏ��łȂ���Ώ��O
			if(tempDistance >= distance) continue;
			//���܂ōł��߂��̂ŋL�^�����
			result = true;
			distance = tempDistance;
			inter = tempInter;
			itHit = it;
		}
	}

	//�ŏI�I�ɂȂɂ��ɓ������Ă����猋�ʂ���������
	if(result && hitInfo)
	{
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *itHit;
		hitInfo->object = hitInfo->collider->GetObject3d();
	}

	return result;

}
