#pragma once

#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();

	//���ƕ��ʂ̏Փ˔���
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	//AABB��AABB�̏Փ˔���
	static bool CheckAABB(const AABB& box1, const AABB& box2);

	/// <summary>
	///�_�ƎO�p�`�̍ŋߐړ_�����߂� 
	/// </summary>
	/// <param name = "point">�_</param>
	/// <param name = "triangle">�O�p�`</param>
	/// <param name = "closest">�ŋߐړ_(�o�͗p)</param>
	static void ClosetPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓�����`�F�b�N
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);

private:

};

