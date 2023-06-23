#include "PostEffect.h"
#include "WindowsAPI.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

//�ÓI�����o�ϐ��̎���
std::string PostEffect::kDefaultTextureDirectoryPath_ = "Resources/";
uint32_t PostEffect::sTextureIndex_;
const size_t kMaxSRVCount = 2056;
//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> PostEffect::textureBuffers_;
std::map<const std::string, uint32_t, std::less<>> PostEffect::textureMap_;
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };	//�΂��ۂ��F

PostEffect::PostEffect() 
	: Sprite()
{
}

void PostEffect::Initialize(DirectXBasic* directXBasic)
{
	HRESULT result;

	directXBasic_ = directXBasic;
	
	//���_�o�b�t�@����
	D3D12_HEAP_PROPERTIES vertexHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC vertResourceDesc = 
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * kVertexCount_);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->vertBuff_));
	assert(SUCCEEDED(result));

	//���_�f�[�^
	vertices[LB] = {
		{ -0.5f , -0.5f, 0.0f }, {0.0f,1.0f}//����
	};
	vertices[LT] = {
		{ -0.5f ,+0.5f, 0.0f }, {0.0f,0.0f}//����
	};
	vertices[RB] = {
		{ +0.5f, -0.5f, 0.0f }, {1.0f,1.0f}//�E��
	};
	vertices[RT] = {
		{ +0.5f, +0.5f, 0.0f }, {1.0f,0.0f}//�E��
	};

	//���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	result = this->vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if(SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		this->vertBuff_->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̍쐬
	this->vbView_.BufferLocation = this->vertBuff_->GetGPUVirtualAddress();
	this->vbView_.SizeInBytes = sizeof(Vertex) * 4;
	this->vbView_.StrideInBytes = sizeof(Vertex);

	//�萔�o�b�t�@�̐���
	D3D12_HEAP_PROPERTIES cbmHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC cbmResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteCommon::ConstBufferDataMaterial) +
			0xff) & ~0xff);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbmHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbmResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffMaterial_));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�Ƀf�[�^�]��
	SpriteCommon::ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = this->constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial);
	if(SUCCEEDED(result))
	{
		constMapMaterial->color = this->color_;
		this->constBuffMaterial_->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@�̐���
	D3D12_HEAP_PROPERTIES cbtHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC cbtResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteCommon::ConstBufferDataTransform) +
			0xff) & ~0xff);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbtHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbtResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffTransform_));
	assert(SUCCEEDED(result));

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowsAPI::kWindow_width_,
		(UINT)WindowsAPI::kWindow_height_,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	D3D12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	D3D12_CLEAR_VALUE textureClearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&textureClearValue,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	//�e�N�X�`����ԃN���A
	//��f��(1280 x 720 = 921600�s�N�Z��)
	const UINT pixelCount = WindowsAPI::kWindow_width_ * WindowsAPI::kWindow_height_;
	//�摜1�s���̃f�[�^�T�C�Y
	const UINT rowPitch = sizeof(UINT) * WindowsAPI::kWindow_width_;
	//�摜�S�̂̃f�[�^�T�C�Y
	const UINT depthPitch = rowPitch * WindowsAPI::kWindow_height_;
	//�摜�C���[�W
	UINT* img = new UINT[pixelCount];
	for(int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff->WriteToSubresource(0, nullptr,
		img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�̃f�X�N���v�^�q�[�v�𐶐�
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};									//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;						//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV����
	directXBasic_->GetDevice()->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//�V�F�[�_�[�̌v�Z���ʂ�SRBG�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//�f�X�N���v�^�q�[�v��RTV�쐬
	directXBasic_->GetDevice()->CreateRenderTargetView(texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WindowsAPI::kWindow_width_,
			WindowsAPI::kWindow_height_,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//�[�x�o�b�t�@�̐���
	D3D12_HEAP_PROPERTIES depthHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_CLEAR_VALUE depthClearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result = directXBasic_->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV���쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;			//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	directXBasic_->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	//�p�C�v���C������
	CreateGraphicsPipelineState();
}

void PostEffect::Draw(const std::string& fileName)
{
	//�����ꂩ�̃L�[�������Ă�����
	//���W���ړ����鏈��(Z���W)
	if(keys_->HasPushedKey(DIK_UP)) { moveSpeed_.y -= 0.1f; }
	else if(keys_->HasPushedKey(DIK_DOWN)) { moveSpeed_.y += 0.1f; }
	else
	{
		moveSpeed_.y = 0.0f;
	}
	if(keys_->HasPushedKey(DIK_RIGHT)) { moveSpeed_.x += 0.1f; }
	else if(keys_->HasPushedKey(DIK_LEFT)) { moveSpeed_.x -= 0.1f; }
	else
	{
		moveSpeed_.x = 0.0f;
	}

	//�萔�o�b�t�@�Ƀf�[�^�]��
	SpriteCommon::ConstBufferDataTransform* constMapTransform = nullptr;
	HRESULT result = this->constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform);
	if(SUCCEEDED(result))
	{
		constMapTransform->mat = XMMatrixIdentity();
		this->constBuffTransform_->Unmap(0, nullptr);
	}
	
	//�p�C�v���C���X�e�[�g�̐ݒ�
	//directXBasic_->GetCommandList()->SetPipelineState(spriteCommon_->GetPipelineState().Get());
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());

	//���[�g�V�O�l�`���̐ݒ�
	//directXBasic_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature_().Get());
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	//�v���~�e�B�u�`���ݒ�
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &this->vbView_);
	//SRV�q�[�v�̐ݒ�R�}���h
	//ID3D12DescriptorHeap* heaps[] = { spriteCommon_->GetSRVHeap() };
	ID3D12DescriptorHeap* heaps[] = { descHeapSRV.Get()};
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//GPU��SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteCommon_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	uint32_t textureIndex;
	textureIndex = spriteCommon_->GetTextureMap().at(fileName);

	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for(uint32_t i = 0; i < textureIndex; i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	//directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, this->constBuffTransform_->GetGPUVirtualAddress());

	if(isInvisible_)
	{
		return;
	}

	//�`��R�}���h(���_���A�C���X�^���X�̐��A�ŏ��̒��_�̃C���f�b�N�X,�f�[�^��ǂݎ��O�Ɋe�C���f�b�N�X�ɒǉ������l)
	directXBasic_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if(FAILED(result_))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if(FAILED(result_))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{	//xyz���W
			
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		
		{	//uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//�O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
	gpipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N�̐ݒ�
	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�X�e�[�g
	gpipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; 
	//�f�v�X�X�e���V���X�e�[�g
	gpipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	 // ��ɏ㏑�����[��

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//�u�����h�X�e�[�g�̐ݒ�
	gpipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	//�[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�(�O�p�`)
	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipelineDesc.NumRenderTargets = 1;								//�`��Ώۂ�1��
	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	gpipelineDesc.SampleDesc.Count = 1;								//1�s�N�Z���ɂ�1��T���v�����O
	
	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);		//t0���W�X�^

	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

	//�X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,&errorBlob);
	assert(SUCCEEDED(result));

	//���[�g�V�O�l�`���̐���
	result = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	gpipelineDesc.pRootSignature = rootSignature.Get();

	//�O���t�B�b�N�X�p�C�v���C���̐���
	result = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

void PostEffect::PreDrawScene()
{
	//���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X�`��\)
	D3D12_RESOURCE_BARRIER resBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(), 
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

	directXBasic_->GetCommandList()->ResourceBarrier(1,&resBarrier);

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	directXBasic_->GetCommandList()->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	D3D12_VIEWPORT viewPort =
		CD3DX12_VIEWPORT(0.0f, 0.0f, WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_);
	directXBasic_->GetCommandList()->RSSetViewports(1, &viewPort);
	//�V�U�����O��`�̐ݒ�
	D3D12_RECT rect =
		CD3DX12_RECT(0, 0, WindowsAPI::kWindow_width_, WindowsAPI::kWindow_height_);
	directXBasic_->GetCommandList()->RSSetScissorRects(1, &rect);

	//�S��ʃN���A
	directXBasic_->GetCommandList()->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//�[�x�o�b�t�@�̃N���A
	directXBasic_->GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	//���\�[�X�o���A��ύX(�`��\���V�F�[�_�[���\�[�X)
	D3D12_RESOURCE_BARRIER resBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	directXBasic_->GetCommandList()->ResourceBarrier(1, &resBarrier);
}

void PostEffect::LoadTexture(const std::string& fileName)
{
	////�摜�ԍ�
	//sTextureIndex_++;

	////�摜�̕�����Ɖ摜�ԍ����i�[
	//textureMap_.emplace(fileName, sTextureIndex_);

	////�f�B���N�g���p�X�ƃt�@�C������A�����𓾂�
	//std::string fullPath = kDefaultTextureDirectoryPath_ + fileName;

	////���C�h������ɕϊ������ۂ̕�����o�b�t�@�T�C�Y���v�Z
	//int32_t filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

	////���C�h������ɕϊ�
	//std::vector<wchar_t> wfilePath(filePathBufferSize);
	//MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

	////�摜�t�@�C���̗p��
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};

	//HRESULT result_ = LoadFromWICFile(
	//	wfilePath.data(),
	//	WIC_FLAGS_NONE,
	//	&metadata, scratchImg);

	//ScratchImage mipChain{};
	////�~�j�}�b�v����
	//result_ = GenerateMipMaps(
	//	scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
	//	TEX_FILTER_DEFAULT, 0, mipChain);
	//if(SUCCEEDED(result_))
	//{
	//	scratchImg = std::move(mipChain);
	//	metadata = scratchImg.GetMetadata();
	//}

	////�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	//metadata.format = MakeSRGB(metadata.format);

	////�q�[�v�ݒ�
	//D3D12_HEAP_PROPERTIES textureHeapProp{};
	//textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	//textureHeapProp.CPUPageProperty =
	//	D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = metadata.format;
	//textureResourceDesc.Width = metadata.width; // ��
	//textureResourceDesc.Height = (UINT)metadata.height; // ��
	//textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	//textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	//textureResourceDesc.SampleDesc.Count = 1;

	////�e�N�X�`���o�b�t�@�̐���
	//result_ = directXBasic_->GetDevice()->CreateCommittedResource(
	//	&textureHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&textureResourceDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&textureBuffers_[sTextureIndex_]));

	///*SetWidth(textureResourceDesc.Width);
	//SetHeight(textureResourceDesc.Height);*/

	////�S�~�j�}�b�v�ɂ���
	//for(size_t i = 0; i < metadata.mipLevels; i++)
	//{
	//	//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
	//	const Image* img = scratchImg.GetImage(i, 0, 0);

	//	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��

	//	result_ = textureBuffers_[sTextureIndex_]->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,
	//		img->pixels,
	//		(UINT)img->rowPitch,
	//		(UINT)img->slicePitch
	//	);

	//	assert(SUCCEEDED(result_));
	//}

	////SRV�q�[�v�̐擪�n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	////�f�X�N���v�^�̃T�C�Y���擾
	//UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	////�擾�����T�C�Y���g�p���ăn���h����i�߂�
	//for(uint32_t i = 0; i < sTextureIndex_; i++)
	//{
	//	srvHandle.ptr += incrementSize;
	//}

	////�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	//memset(&srvDesc, 0, sizeof(srvDesc));
	//srvDesc.Format = textureResourceDesc.Format;//RGBA float
	//srvDesc.Shader4ComponentMapping =
	//	D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	////�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	//directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);
}
