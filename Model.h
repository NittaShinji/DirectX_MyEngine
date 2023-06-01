#pragma once
#include <string>
#include <Vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <map>
#include <array>
#include "DirectXBasic.h"

class Model
{
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void Load(const std::string& path);
	void Update();
	
	static void StaticInitialize(DirectXBasic* directXBasic);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	static void LoadMaterial(const std::string& directoryPath, const std::string& fileName, Model& model);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	static void LoadTexture(const std::string& directoryPath, const std::string& fileName, Model& model);

private:

	static DirectXBasic* directXBasic_;

	struct Material
	{
		std::string name;	//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		// uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDateB0
	{
		XMMATRIX mat;	//3D�ϊ��s��
	};

	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDateB1
	{
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	//static std::string kDefaultTextureDirectoryPath_;

public:

	using MODELKEY = std::string;

	//SRV�̍ő��
	static const size_t kMaxSRVCount = 256;

	struct MODELVALUE
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	//�萔�o�b�t�@

		//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
		ConstBufferDateB0* constMapTransform = nullptr;

		//�萔�o�b�t�@�p�f�[�^
		ConstBufferDateB1* constMap1 = nullptr;

		//�}�e���A��
		Material material_;

		//���_�f�[�^�z��
		std::vector<Vertex> vertices_;
		//���_�C���f�b�N�X�z��
		std::vector<unsigned short> indices_;

		//���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
		//�C���f�b�N�X�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
		//SRV�p�̃f�X�N���v�^�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
		
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		//�C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW ibView{};

		//�e�N�X�`���o�b�t�@
		std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;
	};	

	static const MODELVALUE* GetMODELVALUE(const MODELKEY path);

private:

	//�f�X�N���v�^�q�[�v�n���h��
	static D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	//�e�N�X�`���ԍ�
	static uint32_t textureIndex_;

	//D3D12_RESOURCE_DESC textureResourceDesc_{};
	MODELKEY name_;
	MODELVALUE infomation_;
	static std::map<MODELKEY, MODELVALUE> models_;

public:

	//�Q�b�^�[
	//std::map<MODELKEY, MODELVALUE> GetModels() { return models_; };
	MODELKEY* GetName() { return &name_; };
	MODELVALUE* GetInfomation() { return &infomation_; };
	const uint32_t GetTexIndex() { return textureIndex_; };

	//�Z�b�^�[
	void SetName(MODELKEY name) { name_ = name; };
	void SetInfomation (MODELVALUE infomation) { infomation_ = infomation; };

	/*void SetName(MODELKEY name) { name_ = name; };
	void SetInfomation (MODELVALUE infomation) { infomation_ = infomation; };*/

};

