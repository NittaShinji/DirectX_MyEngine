#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
#include <DirectXTex.h>
#include "Input.h"
using namespace DirectX;

//�X�v���C�g
class Sprite
{
public:

	//������
	void Initialize(SpriteCommon* spriteCommon);
	
	void matUpdate();
	void Update();

	void ImageDateSet();
	void ImageDateSRVSet();

	void TexMapping();
	void TexMappingSRVSet();

	/*void PointListUpdate();
	void LineListUpdate();*/


private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;
	KeyInput* keys_ = nullptr;

	//struct Vertex
	//{
	//	XMFLOAT3 pos;		//xyz���W
	//	XMFLOAT3 normal;	//�@���x�N�g��
	//	XMFLOAT2 uv;		//�摜����xyz���W
	//};

	/*enum VertexName
	{
		LEFTDOWN,
		LEFTCENTER,
		LEFTUP,
		RIGHT
	};*/

	struct Vertex
	{
		XMFLOAT3 pos;// xyz���W
		XMFLOAT2 uv; // uv���W
	};

	//�ˉe�ϊ��s��
	XMMATRIX matProjection;

	//�X�P�[�����O�{��
	XMFLOAT3 scale;
	//��]�p
	float rotationZ;
	//���W
	XMFLOAT3 position;

	//���_�̐�
	static const int vertexCount = 4;
	
	//���_�z��
	std::array<Vertex, vertexCount> vertices{};


	
	// ���_�o�b�t�@�r���[�̍쐬
	//std::array <D3D12_VERTEX_BUFFER_VIEW, imageCount> vbView{};
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	
	// ���_�f�[�^�S�̂̃T�C�Y
	
	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	//�������s�N�Z����
	const size_t textureWidth = 256;
	//�c�����s�N�Z����
	const size_t textureHeight = 256;
	//�z��̗v�f��
	const size_t imageDateCount = textureWidth * textureHeight;

	//�摜�C���[�W�f�[�^�z��
	XMFLOAT4* imageDate;

	//�e�N�X�`���o�b�t�@
	ID3D12Resource* texBuff = nullptr;

	//�e�N�X�`�����\�[�X�f�X�N
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_RESOURCE_DESC resDesc{};

	//�V�F�[�_�[���\�[�X�r���[
	ID3D12DescriptorHeap* srvHeap = nullptr;

public:

	std::array <Vertex, vertexCount> GetterVertex() { return vertices; };

	//�Q�b�^�[
	ID3D12Resource* GetTexBuff() { return texBuff; };

	//ID3D12DescriptorHeap* GetSRVheap() { return srvHeap; };
};
