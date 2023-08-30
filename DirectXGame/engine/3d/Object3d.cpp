#include "Object3d.h"
#include <d3dcompiler.h>
#include "MathUtillity.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

std::string Object3d::kDefaultTextureDirectoryPath_ = "Resources/Model/";
DirectXBasic* Object3d::directXBasic_ = nullptr;
KeyInput* Object3d::keys_ = nullptr;
LightGroup* Object3d::lightGroup_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Object3d::pipelineState_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> Object3d::rootSignature_ = nullptr;

//�萔�o�b�t�@�̐���
template <typename Type1>
ComPtr<ID3D12Resource> Object3d::CrateConstBuff(Type1* directXBasic_)
{
	ComPtr<ID3D12Resource> constBuff_;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(Type1) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
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
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	return constBuff_;
}

void Object3d::StaticInitialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;
	keys_ = KeyInput::GetInstance();
}

void Object3d::Initialize()
{
	scale_ = { 1,1,1 };
	rotation_ = { 0,0,0 };
	transform_ = { 0,0,0 };

	//�N���X���̕�������擾
	name = typeid(*this).name();

	//�萔�o�b�t�@�̐���
	constBuffTransform_ = CrateConstBuff<DirectXBasic>(directXBasic_);
	constBuffMaterial_ = CrateConstBuff<DirectXBasic>(directXBasic_);

	//�萔�o�b�t�@�̃}�b�s���O
	HRESULT result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);//�}�b�s���O
	assert(SUCCEEDED(result));

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
			//�@���x�N�g��
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
	};


#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/Object3DVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);
	// �G���[�Ȃ�
	if(FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Object3DGS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob_, &errorBlob_);
	if(FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma region �s�N�Z���V�F�[�_�[�̓ǂݍ���
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/Object3DPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if(FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc_.GS.pShaderBytecode = gsBlob_->GetBufferPointer();
	pipelineDesc_.GS.BytecodeLength = gsBlob_->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob_->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʂ��J�����O
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc_.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc_.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//��������΍��i
	pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

#pragma region �u�����h�ݒ�
	// �u�����h�X�e�[�g
	for(UINT i = 0; i < kRenderTexNum; i++)
	{
		//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[i];
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
	}

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
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// �p�C�v�����X�e�[�g�̐���
	result = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

Object3d::~Object3d()
{
	if(collider)
	{
		//�R���W�����}�l�[�W���[����o�^����������
		CollisionManager::GetInstance()->RemoveCollider(collider);
	}
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

	cameraPos_ = camera_->GetEye();

	Vector3 move = { 0,0,0 };

	//�X�P�[���A��]�A���s�ړ��̌v�Z
	matScale_ = MatrixIdentity();
	matScale_ = MatrixScale(scale_);
	matRot_ = MatrixIdentity();

	matRot_ *= MatrixRotateZ(rotation_.z);
	matRot_ *= MatrixRotateX(rotation_.x);
	matRot_ *= MatrixRotateY(rotation_.y);

	matTrans_ = MatrixIdentity();
	matTrans_ = MatrixTranslate(transform_);

	matWorld_ = MatrixIdentity();
	matWorld_ *= matScale_;
	matWorld_ *= matRot_;
	matWorld_ *= matTrans_;

	model_.Update();

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform_->worldMatrix = matWorld_;
	constMapTransform_->viewProjection = (matView_ * matProjection_);
	constMapTransform_->cameraPos = cameraPos_;

	//�萔�o�b�t�@�̃}�b�s���O
	HRESULT result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);//�}�b�s���O
	constBuffMaterial_->SetName(L"constBuffMaterial");
	assert(SUCCEEDED(result));

	if(colorFlag_ == false)
	{
		color_ = { 1,1,1};
	}

	SetAmbient(color_);
	constMapMaterial_->diffuse = model_.GetInfomation()->material.diffuse;
	constMapMaterial_->specular = model_.GetInfomation()->material.specular;
	constMapMaterial_->alpha = model_.GetInfomation()->material.alpha;

	constBuffMaterial_->Unmap(0, nullptr);

	//�����蔻��X�V
	if(collider)
	{
		collider->Update();
	}
}

void Object3d::Draw()
{
	std::vector<Mesh>::iterator it;
	for(it = model_.GetInfomation()->meshes.begin(); it < model_.GetInfomation()->meshes.end(); it++)
	{
		//���_�o�b�t�@�r���[�̐ݒ�R�}���h
		directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &it->GetVBView());
		//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		directXBasic_->GetCommandList()->IASetIndexBuffer(&it->GetIBView());
	}

	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { model_.GetInfomation()->srvHeap.Get() };
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
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());

	//���C�g�̕`��
	lightGroup_->Draw(directXBasic_->GetCommandList().Get(), 3);

	//�`��R�}���h
	for(it = model_.GetInfomation()->meshes.begin(); it < model_.GetInfomation()->meshes.end(); it++)
	{
		directXBasic_->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(it->GetIndices().size()), 1, 0, 0, 0);
	}
}

void Object3d::BeforeDraw()
{
	//�p�C�v���C���̃Z�b�g
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	//���[�g�V�O�l�`���̃Z�b�g
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	//�v���~�e�B�u�g�|���W�[�̃Z�b�g
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::AfterDraw(){}

Vector3 Object3d::GetWorldPos() const
{
	//�S�Ă̕ǂ̍��W��n��
	Vector3 resutVec = { 0,0,0 };

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	resutVec.x = matWorld_.m[3][0];
	resutVec.y = matWorld_.m[3][1];
	resutVec.z = matWorld_.m[3][2];

	return resutVec;
}

void Object3d::SetAmbient(Vector3 color)
{
	constMapMaterial_->ambient.x = model_.GetInfomation()->material.ambient.x * color.x;
	constMapMaterial_->ambient.y = model_.GetInfomation()->material.ambient.x * color.y;
	constMapMaterial_->ambient.z = model_.GetInfomation()->material.ambient.x * color.z;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//�R���W�����}�l�[�W���[�ɓo�^
	CollisionManager::GetInstance()->AddCollider(collider);
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}

std::unique_ptr<Object3d> Object3d::Create(const std::string& path)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	std::unique_ptr<Object3d> instance = nullptr;
	instance = std::make_unique<Object3d>();

	if(instance == nullptr)
	{
		return nullptr;
	}

	//���f���ǂݍ���
	Model::Load(path);

	instance->Initialize();
	instance->SetModel(path);

	return instance;
}