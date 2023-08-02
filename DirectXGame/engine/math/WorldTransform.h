#pragma once
#include <DirectXMath.h>

class WorldTransform
{
	using XMFLOAT = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	XMMATRIX worldTransform;

	//�X�P�[��
	XMFLOAT3 scale_;
	//��]�p
	XMFLOAT3 rotation_;
	//���s�ړ�
	XMFLOAT3 transform_;

	XMMATRIX matScale_, matRot_, matTrans_;

	//���[���h�ϊ��s��
	XMMATRIX matWorld_;

	//�r���[�s��
	XMMATRIX matView_;
	//�ˉe�s��
	XMMATRIX matProjection_;

};

