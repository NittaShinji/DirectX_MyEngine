#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
using namespace DirectX;

//�X�v���C�g
class Sprite
{
public:

	//������
	void Initialize(SpriteCommon* spriteCommon);
	
	void Update();

	void ImageDateSet();

	void PointListUpdate();
	void LineListUpdate();


private:

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXBasic* directXBasic_ = nullptr;

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
	
	



public:

	std::array <Vertex, vertexCount> GetterVertex() { return vertices; };

};
