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
	//�V�F�[�_�[�̓ǂݍ���
	void ShaderLoad();
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
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;	//�F(RGBA)
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
	
	//���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//�F�p�̒萔�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ID3D12Resource* constBuffMaterial = nullptr;

	ConstBufferDataMaterial* constMapMaterial = nullptr;
	//���W�p�̒萔�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�O���t�B�b�N�X�p�C�v���C��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�������s�N�Z����
	const size_t textureWidth = 256;
	//�c�����s�N�Z����
	const size_t textureHeight = 256;
	//�z��̗v�f��
	const size_t imageDateCount = textureWidth * textureHeight;
	//�摜�C���[�W�f�[�^�z��
	XMFLOAT4* imageDate;

	//�V�F�[�_�[���\�[�X�r���[
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����

	//�V�F�[�_�[���\�[�X�p�̃f�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* srvHeap_ = nullptr;

	//�e�N�X�`���o�b�t�@
	ID3D12Resource* texBuff = nullptr;


public:

	//�Q�b�^�[
	DirectXBasic* GetDirectXBasic() { return directXBasic_; };
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffMaterial() { return constBuffMaterial; };
	ConstBufferDataMaterial* GetConstBufferDataMaterial() { return constMapMaterial; };
	ID3D12Resource* GetConstBuffTransform() { return constBuffTransform; };
	ConstBufferDataTransform* GetConstMapTransform() { return constMapTransform; };
	
	//�萔�o�b�t�@�̐���
	void CreateConstantBuffer(ID3D12Resource* constBuff);

	//�Z�b�^�[
	//void SetSRVheap(ID3D12DescriptorHeap* srvHeap) { srvHeap_ = srvHeap; };

};