#include "Sprite.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();
	keys_ = KeyInput::GetInstance();

	//�ˉe�ϊ��s��
	//matProjection =
	//	XMMatrixPerspectiveFovLH(
	//		XMConvertToRadians(45.0f),				//�㉺��p45�x
	//		(float)directXBasic_->GetWinWidth() / directXBasic_->GetWinHeight(),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
	//		0.1f, 1000.0f							//�O�[,���[
	//	);

	winWide = directXBasic_->GetWinWidth();
	winHeight = directXBasic_->GetWinHeight();

	scale = { 10.0f,10.0f,10.0f };
	rotation_ = { 0.0f };
	position_ = { 0.0f,0.0f };

	//position_ = { float(winWide / 2),float(winHeight / 2)};

	//�E�C���h�E�̒��S�ɕ\��
	initPosition_ = { float(winWide / 2),float(winHeight / 2) };


	////�X�v���C�g�̍��W
	vertices_.at(LB) = {
		{ initPosition_.x , initPosition_.y + size_.y, 0.0f }, {0.0f,1.0f}//����
	};
	vertices_.at(LT) = {
		{ initPosition_.x ,initPosition_.y, 0.0f }, {0.0f,0.0f}//����
	};
	vertices_.at(RB) = {
		{ initPosition_.x + size_.x, initPosition_.y + size_.y, 0.0f }, {1.0f,1.0f}//�E��
	};
	vertices_.at(RT) = {
		{ initPosition_.x + size_.x, initPosition_.y, 0.0f }, {1.0f,0.0f}//�E��
	};

	////�X�v���C�g�̍��W
	//vertices_.at(LB) = {
	//	{ 0.0f , 0.0f + size_.y, 0.0f }, {0.0f,1.0f}//����
	//};
	//vertices_.at(LT) = {
	//	{ 0.0f ,0.0f, 0.0f }, {0.0f,0.0f}//����
	//};
	//vertices_.at(RB) = {
	//	{ 0.0f + size_.x, 0.0f + size_.y, 0.0f }, {1.0f,1.0f}//�E��
	//};
	//vertices_.at(RT) = {
	//	{ 0.0f + size_.x, 0.0f, 0.0f }, {1.0f,0.0f}//�E��
	//};


	//vertices_.at(LB) = {
	//	{ 0.0f, 100.0f, 0.0f }, {0.0f,1.0f}//����
	//};
	//vertices_.at(LT) = {
	//	{ 0.0f, 0.0f, 0.0f }, {0.0f,0.0f}//����
	//};
	//vertices_.at(RB) = {
	//	{ 100.0f, 100.0f, 0.0f }, {1.0f,1.0f}//�E��
	//};
	//vertices_.at(RT) = {
	//	{ 100.0f, 0.0f, 0.0f }, {1.0f,0.0f}//�E��
	//};



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
	//vbView.StrideInBytes = sizeof(vertices[0]);
	//vbView.StrideInBytes = sizeof(XMFLOAT3);
	vbView.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion
}

void Sprite::matUpdate()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	isFlipX_ = true;
	isFlipY_ = true;

	//���E���]
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	/*float left = (0.0f, - anchorPoint_.x) * initPosition_.x;
	float right = (1.0f, - anchorPoint_.x) * initPosition_.x;
	float top = (0.0f, - anchorPoint_.y) * initPosition_.y;
	float bottom = (1.0f, - anchorPoint_.y) * initPosition_.y;*/

	////���_�f�[�^
	vertices_[LB].pos = { initPosition_.x + left,initPosition_.y + bottom,0.0f };
	vertices_[LT].pos = { initPosition_.x + left,initPosition_.y + top,0.0f };
	vertices_[RB].pos = { initPosition_.x + right,initPosition_.y + bottom,0.0f };
	vertices_[RT].pos = { initPosition_.x + right,initPosition_.y + top,0.0f };

	/*vertices_[LB].pos = { left, bottom,0.0f };
	vertices_[LT].pos = { left,top,0.0f };
	vertices_[RB].pos = { right,bottom,0.0f };
	vertices_[RT].pos = { right,top,0.0f };*/

	//�����ꂩ�̃L�[�������Ă�����
	//���W���ړ����鏈��(Z���W)
	/*if (keys_->HasPushedKey(DIK_UP)) { position_.y -= 0.1f; }
	else if (keys_->HasPushedKey(DIK_DOWN)) { position_.y += 0.1f; }
	else
	{
		position_.y = 0.0f;
	}
	if (keys_->HasPushedKey(DIK_RIGHT)) { position_.x += 0.1f; }
	else if (keys_->HasPushedKey(DIK_LEFT)) { position_.x -= 0.1f; }
	else
	{
		position_.x = 0.0f;
	}*/

	/*vertices_[LB].pos = { 0.0f, size_.y, 0.0f };
	vertices_[LT].pos = { 0.0f, 0.0f, 0.0f };
	vertices_[RB].pos = { size_.x, size_.y, 0.0f };
	vertices_[RT].pos = { size_.x, 0.0f, 0.0f };*/

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

	//size_.x += 0.1f;
	
	//��]
	//rotation_ += 0.01f;

	//vertices_.at(LB) = {
	//	{ 0.0f, size_.y, 0.0f }, {0.0f,1.0f}//���� 
	//};
	//vertices_.at(LT) = {
	//	{ 0.0f, 0.0f, 0.0f }, {0.0f,0.0f}//����
	//};
	//vertices_.at(RB) = {
	//	{ size_.x, size_.y, 0.0f }, {1.0f,1.0f}//�E��
	//};
	//vertices_.at(RT) = {
	//	{ size_.x, 0.0f, 0.0f }, {1.0f,0.0f}//�E��
	//};


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
	matTrans = XMMatrixTranslation(position_.x, position_.y, 0.0f);	//���s�ړ�
	//matTrans = XMMatrixTranslation(initPosition_.x, initPosition_.y, 0.0f);	//���s�ړ�
	//matTrans = XMMatrixTranslation(100.0f, 100.0f, 0.0f);	//���s�ړ�


	matWorld = XMMatrixIdentity();	//�P�ʍs��������ĕό`�����Z�b�g
	//matWorld *= matScale;	//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//�萔�o�b�t�@�Ƀf�[�^�]��
	spriteCommon_->GetConstMapTransform()->mat = matWorld * spriteCommon_->GetConstMapTransform()->mat;
	spriteCommon_->GetConstMapMaterial()->color = color_;
	
}

void Sprite::Update()
{
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//SRV�q�[�v�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, spriteCommon_->GetConstBuffTransform()->GetGPUVirtualAddress());

	if (isInvisible_)
	{
		return;
	}
	//�`��R�}���h(���_���A�C���X�^���X�̐��A�ŏ��̒��_�̃C���f�b�N�X,�f�[�^��ǂݎ��O�Ɋe�C���f�b�N�X�ɒǉ������l)
	directXBasic_->GetCommandList()->DrawInstanced(vertices_.size(), 1, 0, 0);

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

void Sprite::ImageDateSet()
{
	//�摜�t�@�C���̗p��
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WIC�e�N�X�`���̃��[�h
	result_ = LoadFromWICFile(
		L"Resources/reimu.png",	//�uResources�v�t�H���_�́udan.jpg�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//�~�j�}�b�v����
	result_ = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result_))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // ��
	textureResourceDesc.Height = (UINT)metadata.height; // ��
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	//ID3D12Resource* texBuff = nullptr;
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//�S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result_ = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result_));
	}

	//delete[] imageDate;

}

void Sprite::ImageDateSRVSet()
{
	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result_));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXBasic_->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

}

//�e�N�X�`���}�b�s���O�̐ݒ�
void Sprite::TexMapping()
{
	//�摜�C���[�W�f�[�^�z��
	imageDate = new XMFLOAT4[imageDateCount];

	//�S�s�N�Z���̐F��������
	for (size_t i = 0; i < imageDateCount; i++)
	{
		imageDate[i].x = 1.0f; // R
		imageDate[i].y = 0.0f; // G
		imageDate[i].z = 0.0f; // B
		imageDate[i].w = 1.0f; // A
	}

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth; // ��
	textureResourceDesc.Height = textureWidth; // ��
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	//ID3D12Resource* texBuff = nullptr;
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result_ = texBuff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		imageDate,	//���f�[�^�A�h���X
		sizeof(XMFLOAT4) * textureWidth,	// 1���C���T�C�Y
		sizeof(XMFLOAT4) * imageDateCount	//�@�S�T�C�Y
	);

	delete[] imageDate;

	TexMappingSRVSet();
}

void Sprite::TexMappingSRVSet()
{
	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result_));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXBasic_->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

}