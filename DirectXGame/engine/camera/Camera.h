#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <wrl.h>
#include "Vector3.h"

class Camera
{
private:

	//DirectX���ȗ�
	//using XMFLOAT = DirectX::Vector3;
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	static void StaticInitialize(DirectXBasic* directXBasic);

	virtual void Initialize(Vector3& eye, Vector3& target,Vector3& up);

	void Update();

	virtual void UpdateViewMatrix();

	virtual void UpdateProjectionMatrix();

protected:

	static DirectXBasic* directXBasic_;

	//�r���[�ϊ��s��
	XMMATRIX matView_;
	//�r���[�s��̐ݒ荀��
	Vector3 eye_;		//���_���W
	Vector3 target_;	//�����_���W
	Vector3 up_;		//������x�N�g��
	
	float angle_ = 0.0f;	//�J�����̉�]�p

	//�ˉe�s��
	XMMATRIX matProjection_;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

public:

	//�Q�b�^�[
	const XMMATRIX& GetMatView() const { return matView_; }
	const XMMATRIX& GetMatProjection() const { return matProjection_; }
	const Vector3& GetEye() const { return eye_; }

};

