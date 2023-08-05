#pragma once 

#include <DirectXMath.h>
#include "MathUtillity.h"

///<summary>
/// ��
///</summary>
struct Sphere
{
	//���S���W
	Vector3 center = { 0,0,0};
	float upVec = 1.0f;

	//���a
	float radius = 1.0f;

	Vector3 pos = { 0,0,0 };
};

///<summary>
/// ����
///</summary>
struct Plane
{
	//�@���x�N�g��
	Vector3 normal = { 0,1,0 };
	float upVec = 0.0f;

	//���_(0,0,0)����̋���
	float distance = 0.0f;
	Vector3 pos = { 0,0,0 };
};

///<summary>
/// ���C
///</summary>
struct Ray
{
	//�n�_���W
	Vector3 start = { 0,0,0 };
	float upVec = 0.0f;

	//����(�P�ʃx�N�g��)
	Vector3 dir = { 1,0,0};
	float dirUpVec = 0.0f;
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
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;
	//�@���x�N�g��
	Vector3 normal_;
};


