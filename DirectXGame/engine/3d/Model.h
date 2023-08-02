#pragma once
#include "DirectXBasic.h"
#include "Mesh.h"
#include <string>
#include <Vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <map>
#include <array>
#include "Vector2.h"

class Model
{

private:

	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
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
			ambient = { 1.0f,1.0f,1.0f };
			diffuse = { 1.0f,1.0f,1.0f };
			specular = { 1.0f,1.0f, 1.0f };
			alpha = 1.0f;
		}
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

public:

	using MODELKEY = std::string;

	//SRV�̍ő��
	static const size_t kMaxSRVCount_ = 256;

	struct MODELVALUE
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	//�萔�o�b�t�@

		//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
		ConstBufferDateB0* constMapTransform = nullptr;

		//�萔�o�b�t�@�p�f�[�^
		ConstBufferDateB1* constMap1 = nullptr;

		//�}�e���A��
		Material material;

		//���b�V��
		std::vector<Mesh> meshes;

		//SRV�p�̃f�X�N���v�^�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

		//�e�N�X�`���o�b�t�@
		std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers;
	};	

	static const MODELVALUE* GetMODELVALUE(const MODELKEY path);

private:

	//�f�X�N���v�^�q�[�v�n���h��
	static D3D12_CPU_DESCRIPTOR_HANDLE sSrvHandle_;

	//�e�N�X�`���ԍ�
	static uint32_t sTextureIndex_;

	MODELKEY name_;
	MODELVALUE infomation_;
	static std::map<MODELKEY, MODELVALUE> sModels_;

public:

	//�Q�b�^�[
	MODELKEY* GetName() { return &name_; };
	MODELVALUE* GetInfomation() { return &infomation_; };
	uint32_t const GetTexIndex() { return sTextureIndex_; };
	const std::vector<Mesh>& GetMeshes() { return infomation_.meshes; }

	//�Z�b�^�[
	void SetName(const MODELKEY& name) { name_ = name; };
	void SetInfomation (const MODELVALUE& infomation) { infomation_ = infomation; };
};

