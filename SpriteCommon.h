#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
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

	//����������
	void SemiTransparent();
	//���Z����
	void Add();
	//���Z����
	void Sub();
	//�F���]
	void InvertColor();

	//�Q�b�^�[
	DirectXBasic* GetDirectXBasic(){ return directXBasic_; };

private:

	//���_�̐�
	static const int layoutCount = 3;

	// ���_���C�A�E�g
	//std::array<D3D12_INPUT_ELEMENT_DESC,layoutCount> inputLayout{};

	DirectXBasic* directXBasic_ = nullptr;

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
};