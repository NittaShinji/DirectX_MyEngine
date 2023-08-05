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
#include "MathUtillity.h"

using namespace DirectX;
 
class SpriteCommon final
{
private:

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//�F(RGBA)
	};
	struct ConstBufferDataTransform
	{
		Matrix4 mat;
	};

public:

	//������
	void Initialize(DirectXBasic* directXBasic);
	//�X�V
	void Update();

	static SpriteCommon* GetInstance()
	{
		static SpriteCommon spriteCommon;
		return &spriteCommon;
	}

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
	static void LoadTexture(const std::string& fileName);

	//�e���v���[�g�R���X�g���N�^
	template <typename Type1, typename Type2>
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> CrateConstBuff(Type1*& constMapData, Type2* directXBasic_);

private:

	static DirectXBasic* directXBasic_;

	ID3DBlob* vsBlob_ = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob_ = nullptr; // �G���[�I�u�W�F�N�g

	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	
	//���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_{};

	//�F�p�̒萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//ID3D12Resource* constBuffMaterial = nullptr;
	//���W�p�̒萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	//�O���t�B�b�N�X�p�C�v���C��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	//�V�F�[�_�[���\�[�X�p�̃f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//SRV�̍ő��
	static const size_t kMaxSRVCount_ = 2056;

	//�e�N�X�`���o�b�t�@
	static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath_;

	//�e�N�X�`���ԍ�
	static uint32_t sTextureIndex_;
	//�摜�Ɍ��ѕt�����e�N�X�`���ԍ��i�[�pmap
	static std::map<const std::string, uint32_t, std::less<>> textureMap_;

private:

	SpriteCommon();
	~SpriteCommon();

public:

	//�R�s�[�R���X�g���N�^�𖳌�
	SpriteCommon(const SpriteCommon& spritecommon) = delete;
	//������Z�q�𖳌�
	SpriteCommon& operator= (const SpriteCommon& spritecommon) = delete;

public:

	//�Q�b�^�[
	DirectXBasic* GetDirectXBasic() const { return directXBasic_; };
	ComPtr<ID3D12Resource> GetConstBuffMaterial() const { return constBuffMaterial_; };
	ConstBufferDataMaterial* GetConstMapMaterial() const { return constMapMaterial_; };
	ComPtr<ID3D12Resource> GetConstBuffTransform() const { return constBuffTransform_; };
	ConstBufferDataTransform* GetConstMapTransform() const { return constMapTransform_; };
	ID3D12DescriptorHeap* GetSRVHeap() const { return srvHeap_.Get(); };
	const std::map<const std::string, uint32_t, std::less<>>& GetTextureMap() const { return textureMap_; }
	ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState_; };
	ComPtr<ID3D12RootSignature> GetRootSignature_() { return rootSignature_; };


};