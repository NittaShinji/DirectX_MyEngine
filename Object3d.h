#pragma once
#include "DirectXBasic.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
using namespace DirectX;

class Object3d
{
public:
	
	static void StaticInitialize(Microsoft::WRL::ComPtr<ID3D12Device> device, int window_width, int window_hieght);

	void Create();
	void CreateModel();
	void InitializeObject3d(DirectXBasic* directXBasic);
	void Initialize();
	void Update();
	void Draw();

	void RootSignatureSet();

private:

	DirectXBasic* directXBasic_ = nullptr;

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;	//�F(RGBA)
	};
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;	
	};

	struct Vertex
	{
		XMFLOAT3 pos;		// xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		// uv���W
	};


	//�r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye{ 0, 0, -100 };	//���_���W
	XMFLOAT3 target{0, 0, 0};	//�����_���W
	XMFLOAT3 up{ 0, 1, 0 };		//������x�N�g��

	//�ˉe�ϊ��s��
	XMMATRIX matProjection;

	////�X�P�[�����O�{��
	//XMFLOAT3 scale;
	////��]�p
	//XMFLOAT3 rotation;
	////���W
	//XMFLOAT3 position;

	//�F�p�̒萔�o�b�t�@
	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	
	//�萔�o�b�t�@(�s��p)
	ID3D12Resource* constBuffTransform;
	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform;
	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matworld;
	//�e�I�u�W�F�N�g�̃|�C���^
	Object3d* parent = nullptr;

	// 3D�I�u�W�F�N�g�̐�
	static const size_t kObjectCount = 50;
	// 3D�I�u�W�F�N�g�̔z��
	//Object3d object3ds[kObjectCount];

	//���_�z��
	//std::vector<Vertex> vertices_{};

	//XMFLOAT3 vertices_{};

	//uint16_t indices[] =
	//{
	//	0,1,2, //�O�p�`1��
	//	1,2,3, //�O�p�`2��
	//};

	HRESULT result_;
	//���_�f�[�^�z��
	static std::vector<Vertex> vertices_;
	//���_�C���f�b�N�X�z��
	static std::vector<unsigned short> indices_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout{};

	//�e�N�X�`�����\�[�X�f�X�N
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_RESOURCE_DESC resDesc{};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

};
