#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include "MathUtility.h"
#include "Matrix4.h"

#include <wrl.h>

class Camera
{
	//�G�C���A�X�e���v���[�g
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	Camera();

	static void StaticInitialize(DirectXBasic* directXBasic);

	void Initialize(Vector3 eye, Vector3 target, Vector3 up);

	void Updata();

private:

	static DirectXBasic* directXBasic_;

	//�r���[�ϊ��s��
	Matrix4 matView_;
	//�r���[�s��̐ݒ荀��
	Vector3 eye_;		//���_���W
	Vector3 target_;	//�����_���W
	Vector3 up_;		//������x�N�g��
	
	float angle_ = 0.0f;	//�J�����̉�]�p

	// ������������p
	float fovAngleY = 45.0f * MathUtility::PI / 180.0f;
	// �r���[�|�[�g�̃A�X�y�N�g��
	float aspectRatio = (float)16 / 9;
	// �[�x���E�i��O���j
	float nearZ = 0.1f;
	// �[�x���E�i�����j
	float farZ = 1000.0f;

	//�ˉe�s��
	Matrix4 matProjection_;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	//ConstBufferDateTransform* constMapTransform

public:

	//�Q�b�^�[
	const Matrix4 GetMatView() const { return matView_; };
	const Matrix4 GetMatProjection() const { return matProjection_; };

};

