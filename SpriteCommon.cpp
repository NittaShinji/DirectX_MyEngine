#include "SpriteCommon.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

//�����������̏�����
void SpriteCommon::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result_));

	//�萔�o�b�t�@�̃}�b�s���O
	
	result_ = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result_));


	//�l���������ނƎ����I�ɓ]�������
	//constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);	//RGBA�Ŕ������̐�

#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��(P02_01)
	
	
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result_)) {
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
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result_)) {
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

}

void SpriteCommon::SemiTransparent()
{

#pragma region ���_���C�A�E�g(P02_01)
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		//{
		//	//�@���x�N�g��(1�s�ŏ������ق����₷��)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv���W
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�(P02_02_P04)

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�(03_01)
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
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

#pragma endregion

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
	
	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParam.Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����


	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^�[��
	rootSignatureDesc.pParameters = &rootParam;						//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = 1;			//���[�g�p�����[�^�[��
	 
	
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::Add()
{

#pragma region ���_���C�A�E�g(P02_01)
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		//{
		//	//�@���x�N�g��(1�s�ŏ������ق����₷��)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv���W
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�(P02_02_P04)

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�(03_01)
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)

	//RGB�l�̌v�Z���̐ݒ�(���Z����)
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

#pragma endregion

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^�[��
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	//pipelineDesc.pRootSignature = rootSignature.Get();
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::Sub()
{

#pragma region ���_���C�A�E�g(P02_01)
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		//{
		//	//�@���x�N�g��(1�s�ŏ������ق����₷��)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv���W
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�(P02_02_P04)

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�(03_01)
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)

	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��

	
#pragma endregion

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^�[��
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::InvertColor()
{

#pragma region ���_���C�A�E�g(P02_01)
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, //�ǂꂮ�炢�̗ʂ𑗂邩
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		//{
		//	//�@���x�N�g��(1�s�ŏ������ق����₷��)
		//	"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},

		//// uv���W
		//{
		//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		//},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�(P02_02_P04)

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�(03_01)
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)

	//�F���]
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f - �f�X�g�J���[�̒l
	blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�

#pragma endregion

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	//rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^�[��
	//rootSignatureDesc.pStaticSamplers = &samplerDesc;
	//rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

#pragma endregion

}

void SpriteCommon::Update()
{
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}