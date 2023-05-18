#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Input.h"
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <wrl.h>

class Camera
{
	//�G�C���A�X�e���v���[�g
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void StaticInitialize(DirectXBasic* directXBasic);

	void Initialize(XMFLOAT3 eye, XMFLOAT3 target,XMFLOAT3 up);

	void Updata();

private:

	static DirectXBasic* directXBasic_;

	//�r���[�ϊ��s��
	XMMATRIX matView_;
	//�r���[�s��̐ݒ荀��
	XMFLOAT3 eye_;		//���_���W
	XMFLOAT3 target_;	//�����_���W
	XMFLOAT3 up_;		//������x�N�g��
	
	float angle_ = 0.0f;	//�J�����̉�]�p

	//�ˉe�s��
	XMMATRIX matProjection_;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	//ConstBufferDateTransform* constMapTransform

public:

	//�Q�b�^�[
	const XMMATRIX GetMatView() const { return matView_; }
	const XMMATRIX GetMatProjection() const { return matProjection_; }
	const XMFLOAT3 GetEye() const { return eye_; }

};

