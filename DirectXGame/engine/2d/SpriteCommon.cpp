#include "SpriteCommon.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

std::string SpriteCommon::kDefaultTextureDirectoryPath_ = "Resources/Sprite/";
const size_t kMaxSRVCount = 2056;
std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> SpriteCommon::textureBuffers_;
std::map<const std::string, uint32_t, std::less<>> SpriteCommon::textureMap_;
ComPtr<ID3D12DescriptorHeap> SpriteCommon::srvHeap_;
DirectXBasic* SpriteCommon::directXBasic_;
uint32_t SpriteCommon::sTextureIndex_ = 0;

SpriteCommon::SpriteCommon() {}
SpriteCommon::~SpriteCommon(){}

template<typename Type1, typename Type2>
ComPtr<ID3D12Resource> SpriteCommon::CrateConstBuff(Type1*& constMapData, Type2* directXBasic_)
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

void SpriteCommon::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	HRESULT result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result_));
}

void SpriteCommon::ShaderLoad()
{

#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��(P02_01)

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	HRESULT result_ = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);
	// �G���[�Ȃ�
	if(FAILED(result_))
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

#pragma region �s�N�Z���V�F�[�_�[�̓ǂݍ���
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if(FAILED(result_))
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

}

//������
void SpriteCommon::SemiTransparent()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

#pragma region �u�����h�ݒ�
	// �u�����h�X�e�[�g
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result_));

}

//���Z����
void SpriteCommon::Add()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

#pragma region �u�����h�ݒ�
	// �u�����h�X�e�[�g
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result_));

}

//���Z����
void SpriteCommon::Sub()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

#pragma region �u�����h�ݒ�
	// �u�����h�X�e�[�g
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result_));
}

//���]
void SpriteCommon::InvertColor()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

#pragma region �u�����h�ݒ�()
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result_));
}

//�摜�ǂݍ���
void SpriteCommon::LoadTexture(const std::string& fileName)
{
	//�摜�̕�����Ɖ摜�ԍ����i�[
	textureMap_.emplace(fileName, sTextureIndex_);

	//�f�B���N�g���p�X�ƃt�@�C������A�����𓾂�
	std::string fullPath = kDefaultTextureDirectoryPath_ + fileName;

	//���C�h������ɕϊ������ۂ̕�����o�b�t�@�T�C�Y���v�Z
	int32_t filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

	//���C�h������ɕϊ�
	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath.data(), filePathBufferSize);

	//�摜�t�@�C���̗p��
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	HRESULT result_ = LoadFromWICFile(
		wfilePath.data(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	//�~�j�}�b�v����
	result_ = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if(SUCCEEDED(result_))
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
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // ��
	textureResourceDesc.Height = (UINT)metadata.height; // ��
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffers_[sTextureIndex_]));

	//�S�~�j�}�b�v�ɂ���
	for(size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��

		result_ = textureBuffers_[sTextureIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result_));
	}

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for(uint32_t i = 0; i < sTextureIndex_; i++)
	{
		srvHandle.ptr += incrementSize;
	}

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);

	//�摜�ԍ��𑝂₷
	sTextureIndex_++;
}

void SpriteCommon::Update()
{
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	/*directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());*/
}

void SpriteCommon::VertexLayoutSet()
{
	// ���_���C�A�E�g
	inputLayout_ =
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
}

void SpriteCommon::PipelineSet()
{
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob_->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʂ��J�����O���邩�ǂ���
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc_.InputLayout.pInputElementDescs = &inputLayout_.at(0);
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc_.InputLayout.NumElements = static_cast<UINT>(inputLayout_.size());

	// �}�`�̌`��ݒ�
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc_.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

}

void SpriteCommon::RootSignatureSet()
{
	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//�f�X�N���v�^�e�[�u��
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����

	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

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
	//�T���v���[�̐ݒ�����[�g�V�O�l�`���ɒǉ�
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	HRESULT result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result_));
}

void SpriteCommon::DescriptorHeapSet()
{
}

void SpriteCommon::BeforeDraw()
{
	//�p�C�v���C���̃Z�b�g
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState_.Get());
	//���[�g�V�O�l�`���̃Z�b�g
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	//�v���~�e�B�u�g�|���W�[�̃Z�b�g
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�f�X�N���v�^�q�[�v�̃Z�b�g
	DescriptorHeapSet();
}

void SpriteCommon::AfterDraw() {}

void SpriteCommon::TexMapping(int32_t texWidth, int32_t texHeight, Vector4 color, const std::string& fileName)
{
	//�摜�̕�����Ɖ摜�ԍ����i�[
	textureMap_.emplace(fileName, sTextureIndex_);

	//�摜�C���[�W�f�[�^�z��
	Vector4* imageDate;
	//�z��̗v�f��
	const int32_t imageDateCount_ = texWidth * texHeight;
	//�摜�C���[�W�f�[�^�z��
	imageDate = new Vector4[imageDateCount_];

	//�S�s�N�Z���̐F��������
	for(size_t i = 0; i < imageDateCount_; i++)
	{
		imageDate[i].x = color.x; // R
		imageDate[i].y = color.y; // G
		imageDate[i].z = color.z; // B
		imageDate[i].w = color.w; // A
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
	textureResourceDesc.Width = texWidth; // ��
	textureResourceDesc.Height = texHeight; // ��
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffers_[sTextureIndex_]));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result_ = textureBuffers_[sTextureIndex_]->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		imageDate,	//���f�[�^�A�h���X
		sizeof(Vector4) * texWidth,	// 1���C���T�C�Y
		sizeof(Vector4) * imageDateCount_	//�@�S�T�C�Y
	);

	delete[] imageDate;

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for(uint32_t i = 0; i < sTextureIndex_; i++)
	{
		srvHandle.ptr += incrementSize;
	}

	//�V�F�[�_�[���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);

	//�摜�ԍ��𑝂₷
	sTextureIndex_++;
}
