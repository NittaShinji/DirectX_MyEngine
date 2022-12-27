#pragma once
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include <string>

#include "Model.h"
#include "Sprite.h"
#include "Input.h"

using namespace DirectX;

class Object3d
{
public:

	Object3d(const std::string& path, DirectXBasic* directXBasic, uint32_t textureIndex, const std::string& fileName);

	void Update();
	void BeforeDraw();
	void AfterDraw();
	void Draw();
	void LoadTexture(uint32_t textureIndex, const std::string& fileName);
	void SemiTransParent();

private:

	DirectXBasic* directXBasic_ = nullptr;
	Model model_;
	Sprite* sprite_;
	KeyInput* keys_ = nullptr;

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

	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 transform;
	
	std::vector<Vertex> vertex_;

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	//�O���t�B�b�N�X�p�C�v���C��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[���\�[�X�r���[
	ID3D12DescriptorHeap* srvHeap = nullptr;

	//SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;

	//�e�N�X�`���o�b�t�@
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
	uint32_t GetTextureIndex() const { return textureIndex_; };

	//�Z�b�^�[
	void SetTextureIndex(uint32_t textureIndex) { textureIndex_ = textureIndex; };

};
