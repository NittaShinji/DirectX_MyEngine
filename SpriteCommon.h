#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>
#include "DirectXBasic.h"
using namespace DirectX;

class SpriteCommon
{
public:
	
	//������
	void Initialize(DirectXBasic* directXBasic);
	//�X�V
	void Update();

	//���_���C�A�E�g�ݒ�
	void VertexLayoutSet();
	
	//�p�C�v���C���ݒ�
	void PipelineSet();
	//���[�g�V�O�l�`���ݒ�
	void RootSignatureSet();

	//����������
	void SemiTransparent();
	//���Z����
	void Add();
	//���Z����
	void Sub();
	//�F���]
	void InvertColor();

private:

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//�F(RGBA)
	};

	//���_�̐�
	//static const int layoutCount = 6;

	// ���_���C�A�E�g
	//std::array<D3D12_INPUT_ELEMENT_DESC,layoutCount> inputLayout{};

	DirectXBasic* directXBasic_ = nullptr;

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	//ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//�O���t�B�b�N�X�p�C�v���C��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

public:

	//�Q�b�^�[
	DirectXBasic* GetDirectXBasic() { return directXBasic_; };
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffMaterial() { return constBuffMaterial; };
	ConstBufferDataMaterial* GetConstBufferDataMaterial() { return constMapMaterial; };

};