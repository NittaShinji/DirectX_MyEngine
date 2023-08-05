#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <DirectXMath.h>
#include "Matrix4.h"

//���b�V���Փ˔���I�u�W�F�N�g
class MeshCollider : public BaseCollider
{

public:

	MeshCollider()
	{
		//���b�V���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// �O�p�`�̔z����\�z����
	/// </summary>
	/// <param name="model"></param>
	void ConstructTriangles(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă���ۂ�</returns>
	//bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter = nullptr);
	bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr);


	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="distance">����</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	//bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);


private:

	std::vector<Triangle> triangles;
	//���[���h�s��̋t�s��
	//DirectX::XMMATRIX invMatWorld;
	Matrix4 invMatWorld_;


};

