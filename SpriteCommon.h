#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <map>
#include <string>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include "DirectXBasic.h"
using namespace DirectX;
 
class SpriteCommon
{
public:

	~SpriteCommon();
	
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
	//�f�X�N���v�^�q�[�v�̐ݒ�
	void DescriptorHeapSet();

	//�`��J�n�O
	void BeforeDraw();
	//�`��I����
	void AfterDraw();

	//����������
	void SemiTransparent();
	//���Z����
	void Add();
	//���Z����
	void Sub();
	//�F���]
	void InvertColor();
	
	//�摜�ǂݍ���
	void LoadTexture(const std::string& fileName);

	//�s��p�̒萔�o�b�t�@�쐬
	void CrateConstBuffTransform();

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

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	
	//���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//�F�p�̒萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//ID3D12Resource* constBuffMaterial = nullptr;
	//���W�p�̒萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
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

	//�V�F�[�_�[���\�[�X�p�̃f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;

	//�e�N�X�`���o�b�t�@
	static std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath_;

	//�e�N�X�`���������p�ԍ�
	uint32_t textureHandleIndex_;

	//GPU�p��SRV�̃f�X�N���v�^�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//�e�N�X�`���ԍ�
	uint32_t textureIndex_;
	//�摜�Ɍ��ѕt�����e�N�X�`���ԍ��i�[�pmap
	std::map<const std::string, int, std::less<>> textureMap = {};

public:

	//�Q�b�^�[
	DirectXBasic* GetDirectXBasic() { return directXBasic_; };
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffMaterial() { return constBuffMaterial; };
	ConstBufferDataMaterial* GetConstMapMaterial() { return constMapMaterial; };
	ID3D12Resource* GetConstBuffTransform() { return constBuffTransform.Get(); };
	ConstBufferDataTransform* GetConstMapTransform() { return constMapTransform; };
	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); };
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGpuHandle() { return srvGpuHandle; }
	std::map<const std::string, int, std::less<>> GetTextureMap() { return textureMap; }

	//�e���v���[�g�R���X�g���N�^
	template <typename Type1, typename Type2, typename Type3>
	//�萔�o�b�t�@�̐���
	void CrateConstBuff(Type1 *&constBuffer, Type2 *&constMapData, Type3* directXBasic_);

	//�e���v���[�g�R���X�g���N�^
	template <typename Type3, typename Type4, typename Type5>
	//�萔�o�b�t�@�̐���
	void TestCrateConstBuff(Type3 **constBuffer, Type4 *&constMapData, Type5 *directXBasic_);

	//�Z�b�^�[
	//void SetSRVheap(ID3D12DescriptorHeap* srvHeap) { srvHeap_ = srvHeap; };
};