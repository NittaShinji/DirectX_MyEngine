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

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//�ÓI������
	static void StaticInitialize();

	//������
	void Initialize(XMFLOAT2 position, XMFLOAT2 size);
	//�s��X�V
	void matUpdate();
	//�`��
	void Draw(const std::string& fileName);

	void ClearDepthBuffer();

	/*void TexMapping();
	void TexMappingSRVSet();*/

	/*void PointListUpdate();
	void LineListUpdate();*/

protected:

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

	static SpriteCommon* spriteCommon_;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;

	//�E�C���h�E�̒���
	float winWide_;
	float winHeight_;

	//�X�v���C�g�̒���
	float wide_;
	float height_;

	//�X�P�[�����O�{��
	XMFLOAT3 scale_;
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
	static const int32_t kVertexCount_ = 4;

	//�A���J�[�|�C���g
	XMFLOAT2 anchorPoint_;

	//��\���t���O
	bool isInvisible_ = false;
	
	//�����|�W�V����
	XMFLOAT2 position_;

	//���_�z��
	std::array<Vertex, kVertexCount_> vertices_{};

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	
	// ���_�f�[�^�S�̂̃T�C�Y
	
	HRESULT result_;
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	//�e�N�X�`���}�b�s���O�p�̕ϐ�
	//�������s�N�Z����
	const size_t kTextureWidth_ = 256;
	//�c�����s�N�Z����
	const size_t kTextureHeight_ = 256;
	//�z��̗v�f��
	const size_t imageDateCount_ = kTextureWidth_ * kTextureHeight_;
	//�摜�C���[�W�f�[�^�z��
	XMFLOAT4* imageDate_;
	//�e�N�X�`�����\�[�X�f�X�N
	D3D12_RESOURCE_DESC textureResourceDesc_{};

	D3D12_RESOURCE_DESC resDesc_{};

public:

	std::array <Vertex, kVertexCount_> GetterVertex() { return vertices_; };

	//�Q�b�^�[
	const XMFLOAT2& GetMoveSpeed_() const { return moveSpeed_; };
	float GetRotation() const { return rotation_; };
	const XMFLOAT4& GetColor() const { return color_; };
	const XMFLOAT2& GetSize() const { return size_; };
	const XMFLOAT2& GetAnchorPoint() const { return anchorPoint_; };
	const XMFLOAT2& GetPosition() const { return position_; };
	bool GetIsFlipX() const { return isFlipX_; };
	bool GetIsFlipY() const { return isFlipY_; };
	bool GetIsInvisible() const { return isInvisible_; };


	
	//�Z�b�^�[
	void SetMoveSpeed_(const DirectX::XMFLOAT2& moveSpeed) { moveSpeed_ = moveSpeed; };
	void SetRotation(float rotation) { rotation_ = rotation; };
	void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; };
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; };
	void SetAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetPosition(const DirectX::XMFLOAT2& initPosition) { position_ = initPosition; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	//ID3D12DescriptorHeap* GetSRVheap() { return srvHeap; };
	/*void SetWidth(size_t width) { wide_ = wide_; };
	void SetHeight(size_t height) { height_ = height; };*/
};
