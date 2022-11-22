#include "Sprite.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	spriteCommon_ = spriteCommon;
	directXBasic_ = spriteCommon_->GetDirectXBasic();

	// ���_�f�[�^
	Vertex vertices[] = {
		//	x		y	 z			u	v
		// �O
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, -5.0f }, {}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, -5.0f }, {}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, -5.0f }, {}, {1.0f,0.0f}},// �E�� 
		// ��(�O�ʂ�Z���W�̕������t)
		{{ -5.0f, -5.0f, 5.0f }, {}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, 5.0f }, {}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, 5.0f }, {}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f,0.0f}},// �E�� 

		// ��
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, -5.0f },{}, {0.0f,0.0f}},// ���� 
		{{ -5.0f, -5.0f, 5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ -5.0f, 5.0f, 5.0f }, {},{1.0f,0.0f}},// �E�� 

		// �E(���ʂ�X���W�̕������t)
		{{ 5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ 5.0f, 5.0f, -5.0f },{}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, 5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, 5.0f }, {},{1.0f,0.0f}},// �E�� 

		// ��
		{{ -5.0f, -5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, -5.0f, 5.0f },{}, {0.0f,0.0f}},// ���� 
		{{ 5.0f, -5.0f, -5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, -5.0f, 5.0f },{}, {1.0f,0.0f}},// �E�� 
		// ��(���ʂ�Y���W�̕������t)
		{{ -5.0f, 5.0f, -5.0f },{}, {0.0f,1.0f}},// ���� 
		{{ -5.0f, 5.0f, 5.0f },{},{0.0f,0.0f}},// ���� 
		{{ 5.0f, 5.0f, -5.0f },{}, {1.0f,1.0f}},// �E�� 
		{{ 5.0f, 5.0f, 5.0f },{}, {1.0f,0.0f}},// �E�� 

	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
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
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff_->Unmap(0, nullptr);

#pragma region ���_�o�b�t�@�r���[�̍쐬(P02_01)
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	//vbView.StrideInBytes = sizeof(vertices[0]);
	vbView.StrideInBytes = sizeof(XMFLOAT3);


	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
#pragma endregion

#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��(P02_01)
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
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	//D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	//blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	//�A���t�@�l�̌v�Z���̐ݒ�
	//blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��(������`�悵�悤�Ƃ��Ă���F)
	//blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��(���ɕ`����Ă���F)

	//RGB�l�̌v�Z���̐ݒ�(���Z����)
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��

	//�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f - �f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�

	//����������
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f - �\�[�X�̃A���t�@�l

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

	// ���[�g�V�O�l�`��
	//ComPtr<ID3D12RootSignature> rootSignature;
	ID3D12RootSignature* rootSignature;

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
	pipelineDesc.pRootSignature = rootSignature;

	// �p�C�v�����X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelineState;
	ID3D12PipelineState *pipelineState;


	result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

	directXBasic_->GetCommandList()->SetPipelineState(pipelineState);
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature);

	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�`��R�}���h
	directXBasic_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}


