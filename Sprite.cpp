#include "Sprite.h"
#include <d3dcompiler.h>
#include <D3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

//SpriteCommon* Sprite::spriteCommon_ = nullptr;
DirectXBasic* Sprite::directXBasic_ = nullptr;
KeyInput* Sprite::keys_ = nullptr;

//uint32_t Sprite::textureIndex_ = 0;
//const size_t kMaxSRVCount = 2056;
//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> Sprite::textureBuffers_;

//ID3D12DescriptorHeap* Sprite::srvHeap = nullptr;
UINT64 Sprite::fenceCount;
//D3D12_RESOURCE_DESC Sprite::textureResourceDesc{};

void Sprite::StaticInitialize()
{
	keys_ = KeyInput::GetInstance();
}

void Sprite::Initialize( XMFLOAT2 position, XMFLOAT2 size, SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();

	winWide = static_cast<float>(directXBasic_->GetWinWidth());
	winHeight = static_cast<float>(directXBasic_->GetWinHeight());
	//textureHandleIndex_ = textureIndex;

	scale = { 10.0f,10.0f,10.0f };
	rotation_ = { 0.0f };
	moveSpeed_ = { 0.0f,0.0f };
	anchorPoint_ = { 0.0f,0.0f };

	size_ = size;

	//�E�C���h�E�̒��S�ɕ\��
	//initPosition_ = { float(winWide / 2),float(winHeight / 2) };
	position_ = position;

	////�X�v���C�g�̍��W
	//vertices_.at(LB) = {
	//	{ initPosition_.x , initPosition_.y + size_.y, 0.0f }, {0.0f,1.0f}//����
	//};
	//vertices_.at(LT) = {
	//	{ initPosition_.x ,initPosition_.y, 0.0f }, {0.0f,0.0f}//����
	//};
	//vertices_.at(RB) = {
	//	{ initPosition_.x + size_.x, initPosition_.y + size_.y, 0.0f }, {1.0f,1.0f}//�E��
	//};
	//vertices_.at(RT) = {
	//	{ initPosition_.x + size_.x, initPosition_.y, 0.0f }, {1.0f,0.0f}//�E��
	//};
	
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

	/*vertices_.at(LB).pos.y += initPosition.y;
	vertices_.at(RB).pos.x += initPosition.x;
	vertices_.at(RB).pos.y += initPosition.y;
	vertices_.at(RT).pos.x += initPosition.x;*/

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

	//�J���[�̏������݂Ɠ]��
	spriteCommon_->GetConstMapMaterial()->color = XMFLOAT4(0, 1, 0, 0.5f);

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	//ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	result_ = directXBasic_->GetResult();
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	//Vertex* vertMap = nullptr;
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff_->Unmap(0, nullptr);

#pragma region ���_�o�b�t�@�r���[�̍쐬

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
}

void Sprite::matUpdate()
{
	//�A���J�[�|�C���g�̐ݒ�
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	//���]�����@
	//isFlipX_ = true;
	//isFlipY_ = true;

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
	/*vertices_[LB].pos = { initPosition_.x + left,initPosition_.y + bottom,0.0f };
	vertices_[LT].pos = { initPosition_.x + left,initPosition_.y + top,0.0f };
	vertices_[RB].pos = { initPosition_.x + right,initPosition_.y + bottom,0.0f };
	vertices_[RT].pos = { initPosition_.x + right,initPosition_.y + top,0.0f };*/

	vertices_[LB].pos = { left + position_.x , bottom + position_.y,0.0f };
	vertices_[LT].pos = { left + position_.x, top + position_.y,0.0f };
	vertices_[RB].pos = { right + position_.x,bottom + position_.y,0.0f };
	vertices_[RT].pos = { right + position_.x,top + position_.y,0.0f };

	//�����ꂩ�̃L�[�������Ă�����
	//���W���ړ����鏈��(Z���W)
	if (keys_->HasPushedKey(DIK_UP)) { moveSpeed_.y -= 0.1f; }
	else if (keys_->HasPushedKey(DIK_DOWN)) { moveSpeed_.y += 0.1f; }
	else
	{
		moveSpeed_.y = 0.0f;
	}
	if (keys_->HasPushedKey(DIK_RIGHT)) { moveSpeed_.x += 0.1f; }
	else if (keys_->HasPushedKey(DIK_LEFT)) { moveSpeed_.x -= 0.1f; }
	else
	{
		moveSpeed_.x = 0.0f;
	}

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);

	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	XMMATRIX matScale;	//�X�P�[�����O�s��
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	//matScale = XMMatrixScaling(100.0f, 0.5f, 100.0f);

	XMMATRIX matRot;	//��]�s��
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ((rotation_));	//Z������ɉ�]

	XMMATRIX matTrans;	//���s�ړ��s��
	matTrans = XMMatrixTranslation(moveSpeed_.x, moveSpeed_.y, 0.0f);	//���s�ړ�
	
	matWorld = XMMatrixIdentity();	//�P�ʍs��������ĕό`�����Z�b�g
	
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//�萔�o�b�t�@�Ƀf�[�^�]��
	spriteCommon_->GetConstMapTransform()->mat = matWorld * spriteCommon_->GetConstMapTransform()->mat;
	spriteCommon_->GetConstMapMaterial()->color = color_;
}

//void Sprite::Draw(uint32_t textureIndex)
void Sprite::Draw(const std::string& fileName)
{
	spriteCommon_->Update();

	uint32_t textureIndex;
	textureIndex = spriteCommon_->GetTextureMap().at(fileName);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* heaps[] = { spriteCommon_->GetSRVHeap() };
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//GPU��SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteCommon_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

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
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, spriteCommon_->GetConstBuffTransform()->GetGPUVirtualAddress());

	if(isInvisible_)
	{
		return;
	}

	//�`��R�}���h(���_���A�C���X�^���X�̐��A�ŏ��̒��_�̃C���f�b�N�X,�f�[�^��ǂݎ��O�Ɋe�C���f�b�N�X�ɒǉ������l)
	directXBasic_->GetCommandList()->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);

}

Sprite::~Sprite()
{
	
}

//
//void Sprite::TexMapping()
//{
//
//	//�摜�C���[�W�f�[�^�z��
//	XMFLOAT4* imageDate;
//	//�摜�C���[�W�f�[�^�z��
//	imageDate = new XMFLOAT4[imageDateCount];
//
//	//�S�s�N�Z���̐F��������
//	for (size_t i = 0; i < imageDateCount; i++)
//	{
//		imageDate[i].x = 1.0f; // R
//		imageDate[i].y = 0.0f; // G
//		imageDate[i].z = 0.0f; // B
//		imageDate[i].w = 1.0f; // A
//	}
//
//	//�q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty =
//		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	textureResourceDesc.Width = textureWidth; // ��
//	textureResourceDesc.Height = textureWidth; // ��
//	textureResourceDesc.DepthOrArraySize = 1;
//	textureResourceDesc.MipLevels = 1;
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	//�e�N�X�`���o�b�t�@�̐���
//	//ID3D12Resource* texBuff = nullptr;
//	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff));
//
//	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//	result_ = texBuff->WriteToSubresource(
//		0,
//		nullptr,	//�S�̈�փR�s�[
//		imageDate,	//���f�[�^�A�h���X
//		sizeof(XMFLOAT4) * textureWidth,	// 1���C���T�C�Y
//		sizeof(XMFLOAT4) * imageDateCount	//�@�S�T�C�Y
//	);
//
//	delete[] imageDate;
//}
