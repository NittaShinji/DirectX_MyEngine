#pragma once
#include "DirectXBasic.h"
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include <vector>
#include <string>

using namespace DirectX;

class Object3d
{
public:

	//Object3d(DirectXBasic* directXBasic,XMFLOAT3 position);
	Object3d(const std::string& path,XMFLOAT3 position,Model model);

	static void StaticInitialize(DirectXBasic* directXBasic);

	void Update();
	void BeforeDraw();
	void AfterDraw();
	void Draw();
	void LoadTexture(uint32_t textureIndex, const std::string& fileName);
	void SetModel(const std::string& path);

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	Model model_;
	Sprite* sprite_;

	//�萔�o�b�t�@�p�f�[�^�\����B1
	//struct ConstBufferDateB1
	//{
	//	XMFLOAT3 ambient;	//�A���r�G���g�W��
	//	float pad1;
	//	XMFLOAT3 diffuse;
	//	float pad2;
	//	XMFLOAT3 specular;
	//	float alpha;
	//};
	
	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		//XMFLOAT4 color;	//�F(RGBA)
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDateTransform
	{
		XMMATRIX mat;	//3D�ϊ��s��
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		// uv���W
	};
	
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;

	
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDateTransform* constMapTransform = nullptr;
	//ConstBufferDataMaterial* constMapColor = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//�X�P�[��
	XMFLOAT3 scale;
	//��]�p
	XMFLOAT3 rotation;
	//���s�ړ�
	XMFLOAT3 transform;

	//�e�I�u�W�F�N�g�̃|�C���^
	
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	//�O���t�B�b�N�X�p�C�v���C��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[���\�[�X�r���[
	ID3D12DescriptorHeap* srvHeap = nullptr;

	////SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;

	////�e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	//�e�N�X�`�����\�[�X�f�X�N
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC resDesc{};

	//�e�N�X�`���ԍ�
	uint32_t textureIndex_;

	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath_;

public:

	//�Q�b�^�[
	//uint32_t GetTextureIndex() const { return textureIndex_; };

	//�Z�b�^�[
	//void SetTextureIndex(uint32_t textureIndex) { textureIndex_ = textureIndex; };

};
