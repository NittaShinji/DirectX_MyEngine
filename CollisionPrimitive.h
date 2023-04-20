#pragma once 

#include <DirectXMath.h>

///<summary>
/// ��
///</summary>
struct Sphere
{
	//���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;

	DirectX::XMFLOAT3 pos = { 0,0,0 };
};

///<summary>
/// ����
///</summary>
struct Plane
{
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
	DirectX::XMFLOAT3 pos = { 0,0,0 };
};

///<summary>
/// ���C
///</summary>
struct Ray
{
	//�n�_���W
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//����(�P�ʃx�N�g��)
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

/// <summary>
/// �@���t���O�p�`(���v��肪�\��)
/// </summary>
class  Triangle
{
public:

	void ComputeNormal();

public:
	//���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//�@���x�N�g��
	DirectX::XMVECTOR normal;

};


///<summary>
/// AABB
///</summary>
struct AABBScale
{
	DirectX::XMVECTOR max;
	DirectX::XMVECTOR min;
};

struct AABBWidth
{
	DirectX::XMVECTOR max;
	float d[3];
};

struct AABBRadius
{
	DirectX::XMVECTOR center;
	DirectX::XMVECTOR r[3];
};
//
//struct AABB
//{
//	using XMVECTOR = DirectX::XMVECTOR;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//
//	XMFLOAT3 max;
//	XMFLOAT3 min;
//	XMFLOAT3 oldPos;
//	AABB() = default;
//	~AABB() = default;
//	AABB(XMFLOAT3 minValue, XMFLOAT3 maxValue, XMFLOAT3 pos)
//	{
//		min = minValue;
//		max = maxValue;
//		pos = oldPos;
//	}
//	//�X�V
//	void Update(const XMFLOAT3& pos)
//	{
//		XMFLOAT3 diff;
//		diff.x = pos.x - oldPos.x;
//		diff.y = pos.y - oldPos.y;
//		diff.z = pos.z - oldPos.z;
//
//		min.x += diff.x;
//		min.y += diff.y;
//		min.z += diff.z;
//
//		max.x += diff.x;
//		max.y += diff.y;
//		max.z += diff.z;
//
//		oldPos = pos;
//	}
//};


