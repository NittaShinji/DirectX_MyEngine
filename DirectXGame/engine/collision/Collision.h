#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();

	//���ƕ��ʂ̏Փ˔���
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	///�_�ƎO�p�`�̍ŋߐړ_�����߂� 
	/// </summary>
	/// <param name = "point">�_</param>
	/// <param name = "triangle">�O�p�`</param>
	/// <param name = "closest">�ŋߐړ_(�o�͗p)</param>
	static void ClosetPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);


	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓�����`�F�b�N
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);


	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphereA">��</param>
	/// <param name="sphereB">��</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr);


	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="plane">����</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_�p(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, Vector3* inter = nullptr);


	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
};

