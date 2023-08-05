#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;
using namespace MathUtillty;

void MeshCollider::ConstructTriangles(Model* model)
{
	//�O�p�`���X�g���N���A
	triangles.clear();
	//���f���̎����b�V�����X�g���擾
	std::vector<Mesh> meshes = model->GetMeshes();
	//���݂̃��b�V���̊J�n�O�p�`�ԍ������Ă����ϐ�(0�ŏ�����)
	int start = 0;
	//�S���b�V���ɂ��ď��ɏ�������
	std::vector<Mesh>::iterator it = meshes.begin();

	for(; it != meshes.cend(); ++it)
	{
		const std::vector<Mesh::Vertex>& vertices = it->GetVertices();
		const std::vector<unsigned short>& indices = it->GetIndices();

		//�C���f�b�N�X�́A�O�p�`�̐� x 3����̂�
		//�������烁�b�V�����̎O�p�`���t�Z����
		size_t triangleNum = indices.size() / 3;
		//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
		triangles.resize(triangles.size() + triangleNum);
		//�S�O�p�`�ɂ��ď��ɏ�������
		for(int i = 0; i < triangleNum; i++)
		{
			//������v�Z����O�p�`�̎Q��
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];
			//�O�p�`��3���_�̍��W����
			tri.p0_ = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, };
			tri.p1_ = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, };
			tri.p2_ = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, };

			/*tri.p0_ = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, 1 };
			tri.p1_ = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, 1 };
			tri.p2_ = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, 1 };*/
			//3���_����@�����v�Z
			tri.ComputeNormal();
		}

		//���̃��b�V���́A���܂ł̎O�p�`�ԍ��̎�����g��
		start += (int)triangleNum;
	}

}

void MeshCollider::Update()
{
	//���[���h�s��̋t�s����v�Z
	//invMatWorld_ = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
	invMatWorld_ = MatrixInverse(GetObject3d()->GetMatWorld());

}

//bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter)

{
	//�I�u�W�F�N�g�̃��[�J�����W�n�ł̋��𓾂�(���a��X�X�P�[�����Q��)
	Sphere localSphere;
	//localSphere.center = XMVector3Transform(sphere.center, invMatWorld_);
	localSphere.center = Vector3Transform(sphere.center, invMatWorld_)
		;
	//localSphere.radius *= XMVector3Length(invMatWorld_.r[0]).m128_f32[0];
	localSphere.radius *= invMatWorld_.m[0][0];


	//���[�J�����W�n�Ō������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for(; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		//���ƎO�p�`�̓����蔻��
		if(Collision::CheckSphere2Triangle(localSphere, triangle, inter))
		{
			if(inter)
			{
				//const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				const Matrix4& matWorld = GetObject3d()->GetMatWorld();

				//���[���h���W�n�ł̌�_�𓾂�
				//*inter = XMVector3Transform(*inter, matWorld);
				*inter = Vector3Transform(*inter, matWorld);

			}
			return true;
		}
	}



	return false;
}

//bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter)
bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, Vector3* inter)

{
	//�I�u�W�F�N�g�̃��[�J�����W�n�ł̃��C�𓾂�
	Ray localRay;
	localRay.start = Vector3Transform(ray.start, invMatWorld_);
	localRay.dir = Vector3TransformNormal(ray.dir, invMatWorld_);
	localRay.dir = localRay.dir.Normalize();
	/*localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);*/

	//���[�J�����W�n�Ō������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for(; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;

		//XMVECTOR tempInter;
		Vector3 tempInter;

		//���C�ƎO�p�`�̓����蔻��
		if(Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter))
		{
			//const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
			const Matrix4& matWorld = GetObject3d()->GetMatWorld();

			//���[���h���W�n�ł̌�_�𓾂�
			tempInter = Vector3Transform(tempInter, matWorld);
			//tempInter = XMVector3Transform(tempInter, matWorld);


			if(distance)
			{
				//��_�ƃ��C�n�_�̋������v�Z
				Vector3 sub = tempInter - ray.start;
				*distance = Vector3Dot(sub, ray.dir).x;
				/*XMVECTOR sub = tempInter - ray.start;
				*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];*/
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
