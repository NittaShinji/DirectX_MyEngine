#include "Sprite.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();

	// ���_�f�[�^

	//���_�f�[�^
	vertices.at(0) = {
		{ -0.5f, -0.5f, 0.0f },//����
	};
	vertices.at(1) = {
		{ -0.5f, +0.5f, 0.0f },//����
	};
	vertices.at(2) = {
		{ +0.5f, -0.5f, 0.0f },//�E��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPos) * vertices.size());

	//�J���[
	spriteCommon_->GetConstBufferDataMaterial()->color = XMFLOAT4(0, 1, 0, 0.5f);

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	result_ = directXBasic_->GetResult();
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//Vertex* vertMap = nullptr;
	VertexPos* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff_->Unmap(0, nullptr);

#pragma region ���_�o�b�t�@�r���[�̍쐬(P02_01)
	
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	//vbView.StrideInBytes = sizeof(vertices[0]);
	vbView.StrideInBytes = sizeof(XMFLOAT3);

#pragma endregion
}

void Sprite::Update()
{
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, spriteCommon_->GetConstBuffMaterial()->GetGPUVirtualAddress());

	//�`��R�}���h(���_���A�C���X�^���X�̐��A�ŏ��̒��_�̃C���f�b�N�X,�f�[�^��ǂݎ��O�Ɋe�C���f�b�N�X�ɒǉ������l)
	directXBasic_->GetCommandList()->DrawInstanced(vertices.size(), 1, 0, 0);
}
