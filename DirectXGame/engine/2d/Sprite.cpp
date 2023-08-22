#include "Sprite.h"
#include "WindowsAPI.h"
#include <d3dcompiler.h>
#include <D3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace MathUtillty;

SpriteCommon* Sprite::spriteCommon_ = nullptr;
TextureManager* Sprite::textureManager_ = nullptr;
DirectXBasic* Sprite::directXBasic_ = nullptr;
KeyInput* Sprite::keys_ = nullptr;
Matrix4 Sprite::matProjection_;

template<typename Type1, typename Type2>
ComPtr<ID3D12Resource> Sprite::CrateConstBuff(Type1*& constMapData, Type2* directXBasic_)
{
	ComPtr<ID3D12Resource> constBuff_;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(Type2) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O
	result_ = constBuff_->Map(0, nullptr, (void**)&constMapData);//�}�b�s���O
	assert(SUCCEEDED(result_));

	return constBuff_;
}

void Sprite::StaticInitialize()
{
	keys_ = KeyInput::GetInstance();
	spriteCommon_ = SpriteCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();
}

void Sprite::Initialize( Vector2 position, Vector2 size)
{
	directXBasic_ = spriteCommon_->GetDirectXBasic();

	winWide_ = static_cast<float>(directXBasic_->GetWinWidth());
	winHeight_ = static_cast<float>(directXBasic_->GetWinHeight());

	//�萔�o�b�t�@�̐���
	constBuffMaterial_ = CrateConstBuff<SpriteCommon::ConstBufferDataMaterial, DirectXBasic>(constMapMaterial_, directXBasic_);
	constBuffTransform_ = CrateConstBuff<SpriteCommon::ConstBufferDataTransform, DirectXBasic>(constMapTransform_, directXBasic_);

	//�P�ʍs�����
	constMapTransform_->mat = MatrixIdentity();

	constMapTransform_->mat.m[0][0] = 2.0f / directXBasic_->GetWinWidth();		//�E�C���h�E����
	constMapTransform_->mat.m[1][1] = -2.0f / directXBasic_->GetWinHeight();	//�E�C���h�E�c��
	//��ʔ����̕��s�ړ�
	constMapTransform_->mat.m[3][0] = -1.0f;
	constMapTransform_->mat.m[3][1] = 1.0f;


	scale_ = { 10.0f,10.0f,10.0f };
	rotation_ = { 0.0f };
	moveSpeed_ = { 0.0f,0.0f };
	anchorPoint_ = { 0.0f,0.0f };

	size_ = size;

	//�E�C���h�E�̒��S�ɕ\��
	position_ = position;

	//�X�v���C�g�̍��W
	vertices_.at(LB) = {
		{ 0.0f , size_.y, 0.0f }, {0.0f,1.0f}//����
	};
	vertices_.at(LT) = {
		{ 0.0f ,0.0f, 0.0f }, {0.0f,0.0f}//����
	};
	vertices_.at(RB) = {
		{ size_.x, size_.y, 0.0f }, {1.0f,1.0f}//�E��
	};
	vertices_.at(RT) = {
		{ size_.x, 0.0f, 0.0f }, {1.0f,0.0f}//�E��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	//�J���[�̏������݂Ɠ]��
	//spriteCommon_->GetConstMapMaterial()->color = Vector4(0, 1, 0, 0.5f);
	constMapMaterial_->color = Vector4(0, 1, 0, 0.5f);


	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	result_ = directXBasic_->GetResult();
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	for (int32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}

	//TransferVertices();

	// �q���������
	//vertBuff_->Unmap(0, nullptr);

#pragma region ���_�o�b�t�@�r���[�̍쐬

	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion

	// �ˉe�s��v�Z
	matProjection_ =
		Matrix4Orthographic(
			0.0f, WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_, 0.0f, 0.0f, 1.0f);
}

void Sprite::matUpdate()
{
	//�A���J�[�|�C���g�̐ݒ�
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//���E���]
	if (isFlipX_ == true)
	{
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY_ == true)
	{
		top = -top;
		bottom = -bottom;
	}

	//���_�f�[�^
	vertices_[LB].pos = { left,bottom ,0.0f };
	vertices_[LT].pos = { left,top ,0.0f };
	vertices_[RB].pos = { right,bottom ,0.0f };
	vertices_[RT].pos = { right,top ,0.0f };

	/*vertices_[LB].pos = { left,bottom ,0.0f };
	vertices_[LT].pos = { left,top ,0.0f };
	vertices_[RB].pos = { right,bottom ,0.0f };
	vertices_[RT].pos = { right,top ,0.0f };*/

	/*vertices_[LB].pos = { left + position_.x , bottom + position_.y,0.0f };
	vertices_[LT].pos = { left + position_.x, top + position_.y,0.0f };
	vertices_[RB].pos = { right + position_.x,bottom + position_.y,0.0f };
	vertices_[RT].pos = { right + position_.x,top + position_.y,0.0f };
	*/
	// �e�N�X�`�����擾
	
	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	for (int32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	//vertBuff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	//memcpy(vertMap_, vertices, sizeof(vertices));


	Matrix4 matScale;	//�X�P�[�����O�s��
	matScale = MatrixScale(scale_);

	Matrix4 matRot;	//��]�s��
	matRot = MatrixIdentity();
	//matRot *= MatrixRotateZ(ToRadian(180.0f));	//Z������ɉ�]
	matRot *= MatrixRotateZ(rotation_);	//Z������ɉ�]
	//matRot *= MatrixRotateX(ToRadian(0.0f));	//X������ɉ�]
	//matRot *= MatrixRotateY(ToRadian(0.0f));	//Y������ɉ�]

	Matrix4 matTrans;	//���s�ړ��s��
	matTrans = MatrixIdentity();
	//matTrans *= MatrixTranslate(Vector3(moveSpeed_.x,moveSpeed_.y, 0.0f));	//���s�ړ�
	matTrans *= MatrixTranslate(Vector3(position_.x,position_.y, 0.0f));	//���s�ړ�


	//���[���h�ϊ��s��
	Matrix4 matWorld;
	matWorld = MatrixIdentity();	//�P�ʍs��������ĕό`�����Z�b�g
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//�萔�o�b�t�@�Ƀf�[�^�]��
	constMapTransform_->mat = matWorld * matProjection_;
	constMapMaterial_->color = color_;
	/*spriteCommon_->GetConstMapTransform()->mat = matWorld * matProjection_;
	spriteCommon_->GetConstMapMaterial()->color = color_;*/



	moveSpeed_.x = 0.0f;
	moveSpeed_.y = 0.0f;
}

void Sprite::Draw(const std::string& fileName)
{
	spriteCommon_->Update();

	uint32_t textureIndex;
	//textureIndex = spriteCommon_->GetTextureMap().at(fileName);
	textureIndex = textureManager_->GetTextureMap().at(fileName);


	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//SRV�q�[�v�̐ݒ�R�}���h
	//ID3D12DescriptorHeap* heaps[] = { spriteCommon_->GetSRVHeap() };
	ID3D12DescriptorHeap* heaps[] = { textureManager_->GetSRVHeap() };

	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//GPU��SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteCommon_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = textureManager_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();


	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for(uint32_t i = 0; i < textureIndex; i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());
	/*directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, spriteCommon_->GetConstBuffTransform()->GetGPUVirtualAddress());*/


	if(isInvisible_)
	{
		return;
	}

	//�`��R�}���h(���_���A�C���X�^���X�̐��A�ŏ��̒��_�̃C���f�b�N�X,�f�[�^��ǂݎ��O�Ɋe�C���f�b�N�X�ɒǉ������l)
	directXBasic_->GetCommandList()->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;
	if(isFlipX_)
	{ // ���E����ւ�
		left = -left;
		right = -right;
	}

	if(isFlipY_)
	{ // �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	Vertex vertices[kVertexCount_];

	vertices[LB].pos = { left, bottom, 0.0f };  // ����
	vertices[LT].pos = { left, top, 0.0f };     // ����
	vertices[RB].pos = { right, bottom, 0.0f }; // �E��
	vertices[RT].pos = { right, top, 0.0f };    // �E��

	//// �e�N�X�`�����擾
	//{
	//	float tex_left = texBase_.x / resourceDesc_.Width;
	//	float tex_right = (texBase_.x + texSize_.x) / resourceDesc_.Width;
	//	float tex_top = texBase_.y / resourceDesc_.Height;
	//	float tex_bottom = (texBase_.y + texSize_.y) / resourceDesc_.Height;

	//	vertices[LB].uv = { tex_left, tex_bottom };  // ����
	//	vertices[LT].uv = { tex_left, tex_top };     // ����
	//	vertices[RB].uv = { tex_right, tex_bottom }; // �E��
	//	vertices[RT].uv = { tex_right, tex_top };    // �E��
	//}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	memcpy(vertMap_, vertices, sizeof(vertices));
}

void Sprite::Update()
{
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
}
