#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <array>
#include "SpriteCommon.h"
#include "Input.h"

//�X�v���C�g
class Sprite
{
private:

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	
	//�ÓI������
	static void StaticInitialize();

	//������
	void Initialize(Vector2 position, Vector2 size);
	//�s��X�V
	void matUpdate();
	//�`��
	void Draw(const std::string& fileName);

	//void TexMapping();

	void TransferVertices();

	void Update();

	//�e���v���[�g�R���X�g���N�^
	template <typename Type1, typename Type2>
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> CrateConstBuff(Type1*& constMapData, Type2* directXBasic_);

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
		Vector3 pos;// xyz���W
		Vector2 uv; // uv���W
	};

	static SpriteCommon* spriteCommon_;

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;

	//�E�C���h�E�̒���
	float winWide_;
	float winHeight_;

	//�X�v���C�g�̒���
	UINT width_;
	UINT height_;

	//�X�P�[�����O�{��
	Vector3 scale_;
	//��]�p
	float rotation_;
	//���W
	//Vector3 position;
	Vector2 moveSpeed_;
	//�F(RGBA)
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//�\���T�C�Y
	Vector2 size_ = { 100.0f,100.0f };

	//���E�t���b�v
	bool isFlipX_ = false;
	//�㉺�t���b�v
	bool isFlipY_ = false;

	//���_�̐�
	static const int32_t kVertexCount_ = 4;

	//�A���J�[�|�C���g
	Vector2 anchorPoint_;

	//��\���t���O
	bool isInvisible_ = false;
	
	//�����|�W�V����
	Vector2 position_;

	//���_�z��
	std::array<Vertex, kVertexCount_> vertices_{};

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	
	// ���_�f�[�^�S�̂̃T�C�Y
	HRESULT result_;
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	
	//�e�N�X�`���}�b�s���O�p�̕ϐ�
	//�������s�N�Z����
	//const size_t kTextureWidth_ = 256;
	//�c�����s�N�Z����
	//const size_t kTextureHeight_ = 256;
	//�z��̗v�f��
	//const size_t imageDateCount_ = kTextureWidth_ * kTextureHeight_;
	//�摜�C���[�W�f�[�^�z��
	Vector4* imageDate_;
	//�e�N�X�`�����\�[�X�f�X�N
	D3D12_RESOURCE_DESC textureResourceDesc_{};

	D3D12_RESOURCE_DESC resDesc_{};

	// �ˉe�s��
	static Matrix4 matProjection_;

	// ���_�o�b�t�@�}�b�v
	Vertex* vertMap_ = nullptr;

	//�F�p�̒萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//ID3D12Resource* constBuffMaterial = nullptr;
	//���W�p�̒萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	SpriteCommon::ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	SpriteCommon::ConstBufferDataTransform* constMapTransform_ = nullptr;


public:

	std::array <Vertex, kVertexCount_> GetterVertex() { return vertices_; };

	//�Q�b�^�[
	const Vector2& GetMoveSpeed_() const { return moveSpeed_; };
	float GetRotation() const { return rotation_; };
	const Vector4& GetColor() const { return color_; };
	const Vector2& GetSize() const { return size_; };
	const Vector2& GetAnchorPoint() const { return anchorPoint_; };
	const Vector2& GetPosition() const { return position_; };
	bool GetIsFlipX() const { return isFlipX_; };
	bool GetIsFlipY() const { return isFlipY_; };
	bool GetIsInvisible() const { return isInvisible_; };

	//�Z�b�^�[
	void SetMoveSpeed_(const Vector2& moveSpeed) { moveSpeed_ = moveSpeed; };
	void SetRotation(float rotation) { rotation_ = rotation; };
	void SetColor(const Vector4& color) { color_ = color; };
	void SetSize(const Vector2& size) { size_ = size; };
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetPosition(const Vector2& initPosition) { position_ = initPosition; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; };
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; };
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; };
	void SetWidth(UINT width) { width_ = width; }
	void SetHeight(UINT height) { height_ = height; }
};
