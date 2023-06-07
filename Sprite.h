#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
#include "Input.h"
using namespace DirectX;

//�X�v���C�g
class Sprite
{
public:

	//�ÓI�������v
	static void StaticInitialize();

	//������
	void Initialize(XMFLOAT2 position, XMFLOAT2 size, SpriteCommon* spriteCommon);
	//�s��X�V
	void matUpdate();
	//�`��
	//void Draw(uint32_t textureIndex);
	void Draw(const std::string& fileName);


	~Sprite();

	/*void TexMapping();
	void TexMappingSRVSet();*/

	/*void PointListUpdate();
	void LineListUpdate();*/

private:

	SpriteCommon* spriteCommon_;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;

	enum VertexNumber
	{
		LB,//�E��
		LT,//����
		RB,//�E��
		RT,//�E��
	};

	struct Vertex
	{
		XMFLOAT3 pos;// xyz���W
		XMFLOAT2 uv; // uv���W
	};

	//�E�C���h�E�̒���
	float winWide;
	float winHeight;

	//�X�v���C�g�̒���
	float wide_;
	float height_;

	//�ˉe�ϊ��s��
	XMMATRIX matProjection;

	//�X�P�[�����O�{��
	XMFLOAT3 scale;
	//��]�p
	float rotation_;
	//���W
	//XMFLOAT3 position;
	XMFLOAT2 moveSpeed_;
	//�F(RGBA)
	XMFLOAT4 color_ = { 1,1,1,1 };
	//�\���T�C�Y
	XMFLOAT2 size_ = { 100.0f,100.0f };

	//���E�t���b�v
	bool isFlipX_ = false;
	//�㉺�t���b�v
	bool isFlipY_ = false;

	//���_�̐�
	static const int vertexCount = 4;

	//�A���J�[�|�C���g
	XMFLOAT2 anchorPoint_;

	//��\���t���O
	bool isInvisible_ = false;
	
	//�����|�W�V����
	XMFLOAT2 position_;

	//���_�z��
	std::array<Vertex, vertexCount> vertices_{};

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

	
	
	//static std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> uploadBuffer;

	//�e�N�X�`�����\�[�X�f�X�N
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_RESOURCE_DESC resDesc{};

	//�V�F�[�_�[���\�[�X�r���[
	//ID3D12DescriptorHeap* srvHeap = nullptr;
	//static ID3D12DescriptorHeap* srvHeap;

	//�e�N�X�`���ԍ�
	//uint32_t textureIndex_;
	//�e�N�X�`���������p�ԍ�
	//uint32_t textureHandleIndex_;

	static UINT64 fenceCount;

public:

	std::array <Vertex, vertexCount> GetterVertex() { return vertices_; };

	//�Q�b�^�[
	//ID3D12Resource* GetTexBuff() { return texBuff; };
	const XMFLOAT2& GetMoveSpeed_() const { return moveSpeed_; };
	float GetRotation() const { return rotation_; };
	const XMFLOAT4 GetColor() const { return color_; };
	const XMFLOAT2 GetSize() const { return size_; };
	const XMFLOAT2 GetAnchorPoint() const { return anchorPoint_; };
	const XMFLOAT2 GetPosition() const { return position_; };
	bool GetIsFlipX() const { return isFlipX_; };
	bool GetIsFlipY() const { return isFlipY_; };
	bool GetIsInvisible() const { return isInvisible_; };
	//uint32_t GetTextureIndex() const { return textureIndex_; };
	
	//�Z�b�^�[
	void SetMoveSpeed_(const DirectX::XMFLOAT2& moveSpeed) { moveSpeed_ = moveSpeed; };
	void SetRotation(float rotation) { rotation_ = rotation; };
	void SetColor(const DirectX::XMFLOAT4 color) { color_ = color; };
	//ID3D12DescriptorHeap* GetSRVheap() { return srvHeap; };
	void SetSize(const DirectX::XMFLOAT2 size) { size_ = size; };
	void SetAnchorPoint(const DirectX::XMFLOAT2 anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetPosition(const DirectX::XMFLOAT2 initPosition) { position_ = initPosition; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	//void SetTextureIndex(uint32_t textureIndex) { textureIndex_ = textureIndex; };
	/*void SetWidth(size_t width) { wide_ = wide_; };
	void SetHeight(size_t height) { height_ = height; };*/

};
