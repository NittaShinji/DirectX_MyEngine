#include "Object3d.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

std::string Object3d::kDefaultTextureDirectoryPath_ = "Resources/";
DirectXBasic* Object3d::directXBasic_ = nullptr;
KeyInput* Object3d::keys_ = nullptr;
LightGroup* Object3d::lightGroup_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Object3d::pipelineState = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> Object3d::rootSignature = nullptr;

//�萔�o�b�t�@�̐���
template <typename Type1, typename Type2, typename Type3>
void Object3d::CrateConstBuff(Type1 *&constBuffer, Type3* directXBasic_)
{
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
	HRESULT result = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer));
	assert(SUCCEEDED(result));
}

void Object3d::CrateConstBuffandMapping()
{
}

void Object3d::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
}

void Object3d::Initialize(const std::string& path, XMFLOAT3 position, XMFLOAT3 Modelscale)
{
	scale = Modelscale;
	rotation = { 0.0f,0.0f,0.0f };
	transform = position;


	/*scale = Modelscale;
	rotation = { 0.0f,0.0f,0.0f };
	transform = position;*/

	ID3D12Resource* constBuffTransform_ = constBuffTransform.Get();
	ID3D12Resource* constBuffMaterial_ = constBuffMaterial.Get();
	ID3D12Resource* constBuffLight_ = constBuffLight.Get();


	CrateConstBuff<ID3D12Resource, DirectXBasic>(constBuffTransform_, directXBasic_);
	constBuffTransform = constBuffTransform_;

	//�萔�o�b�t�@�̃}�b�s���O
	HRESULT result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//�}�b�s���O
	assert(SUCCEEDED(result));

	//�P�ʍs�����
	//constMapTransform->mat = XMMatrixIdentity();

	CrateConstBuff<ID3D12Resource, DirectXBasic>(constBuffMaterial_, directXBasic_);
	constBuffMaterial = constBuffMaterial_;

	CrateConstBuff<ID3D12Resource, DirectXBasic>(constBuffLight_, directXBasic_);
	constBuffLight = constBuffLight_;

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffLight->Map(0, nullptr, (void**)&constMapLight);//�}�b�s���O
	assert(SUCCEEDED(result));

	constMapLight->lightv = { 0,0,1 };
	constMapLight->lightcolor = { 1,1,1 };

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC  inputLayout[] =
	{
		{
			//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			//�@���x�N�g��(1�s�ŏ������ق����₷��)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		// uv���W
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};


#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��(P02_01)

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/Object3DVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if(FAILED(result))
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
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�[�̓ǂݍ���
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/Object3DPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if(FAILED(result))
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

#pragma endregion

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

#pragma region �u�����h�ݒ�
	// �u�����h�X�e�[�g

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l

	//#pragma endregion

		//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[4] = {};

	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

	//�萔�o�b�t�@1��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[1].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[1].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

	//�e�N�X�`�����W�X�^0��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//�f�X�N���v�^�e�[�u��
	rootParams[2].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[2].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����

	//�萔�o�b�t�@1��
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[3].Descriptor.ShaderRegister = 2;					//�萔�o�b�t�@�ԍ�
	rootParams[3].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����


	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�j�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�j�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^�[��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	result = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	model_.SetName(path);
	model_.SetInfomation(*Model::GetMODELVALUE(path));
}

Object3d::~Object3d()
{
	delete sprite_;
}

Object3d::Object3d()
{
	
}

void Object3d::SetModel(const std::string& path)
{
	//�����L�[���烂�f�������������A���f�������蓖�Ă�
	model_.SetName(path);
	model_.SetInfomation(*Model::GetMODELVALUE(path));
}

void Object3d::Update(Camera* camera)
{
	//�g�p����J�������Z�b�g
	camera_ = camera;

	//�r���[�ϊ��s��
	matView_ = camera_->GetMatView();
	//�ˉe�ϊ��s��
	matProjection_ = camera_->GetMatProjection();

	cameraPos = camera_->GetEye();

	XMFLOAT3 move = { 0,0,0 };

	//�����ꂩ�̃L�[�������Ă�����
	//���W���ړ����鏈��
	if(keys_->HasPushedKey(DIK_RIGHT)) { transform.x += 0.4f; }
	else if(keys_->HasPushedKey(DIK_LEFT)) { transform.x -= 0.4f; }
	else {}
	if(keys_->HasPushedKey(DIK_P)) { transform.y += 0.4f; }
	else if(keys_->HasPushedKey(DIK_L)) { transform.y -= 0.4f; }
	else {}

	
	matWorld = XMMatrixIdentity();

	//�X�P�[���A��]�A���s�ړ��̌v�Z
	matScale = XMMatrixIdentity();
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixIdentity();
	matTrans = XMMatrixTranslation(transform.x, transform.y, transform.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	model_.Update();

	/*if(colorFlag_ == true)
	{
		constMapTransform->color = { 1,0,0,1 };
	}
	else if(colorFlag_ == false)
	{
		constMapTransform->color = { 1,1,1,1 };
	}*/

	//�萔�o�b�t�@�փf�[�^�]��
	//mat�������ƕ\�������������Ȃ��Ă��܂�
	//constMapTransform->mat = matWorld * matView_ * matProjection_;
	
	constMapTransform->worldMatrix = matWorld;
	constMapTransform->viewProjection = (matView_ * matProjection_);
	constMapTransform->cameraPos = cameraPos;

	//�萔�o�b�t�@�̃}�b�s���O
	HRESULT result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	constBuffMaterial->SetName(L"constBuffMaterial");
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = model_.GetInfomation()->material_.ambient;
	constMapMaterial->diffuse = model_.GetInfomation()->material_.diffuse;
	constMapMaterial->specular = model_.GetInfomation()->material_.specular;
	constMapMaterial->alpha = model_.GetInfomation()->material_.alpha;

	constBuffMaterial->Unmap(0, nullptr);
	
}

void Object3d::Draw()
{
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &model_.GetInfomation()->vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetIndexBuffer(&model_.GetInfomation()->ibView);

	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { model_.GetInfomation()->srvHeap.Get()};
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, ppHeaps);

	//GPU��SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = model_.GetInfomation()->srvHeap->GetGPUDescriptorHandleForHeapStart();
	
	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for(uint32_t i = 0; i < model_.GetTexIndex(); i++)
	{
		srvGpuHandle.ptr += incrementSize;
	}

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃ̃f�X�N���v�^�����W�ɐݒ�
	directXBasic_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffMaterial->GetGPUVirtualAddress());
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuffLight->GetGPUVirtualAddress());

	//���C�g�̕`��
	lightGroup_->Draw(directXBasic_->GetCommandList().Get(), 3);

	//�`��R�}���h
	directXBasic_->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(model_.GetInfomation()->indices_.size()), 1, 0, 0, 0);
}

void Object3d::BeforeDraw()
{
	//�p�C�v���C���̃Z�b�g
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̃Z�b�g
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�g�|���W�[�̃Z�b�g
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::AfterDraw()
{

}

XMFLOAT3 Object3d::GetWorldPos()
{
	//�S�Ă̕ǂ̍��W��n��
	XMFLOAT3 resutVec = { 0,0,0 };

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	resutVec.x = matWorld.r[3].m128_f32[0];
	resutVec.y = matWorld.r[3].m128_f32[1];
	resutVec.z = matWorld.r[3].m128_f32[2];

	return resutVec;

}

void Object3d::Create(Model* model)
{

}