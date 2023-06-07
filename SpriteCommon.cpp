#include "SpriteCommon.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

std::string SpriteCommon::kDefaultTextureDirectoryPath_ = "Resources/";
const size_t kMaxSRVCount = 2056;

std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> SpriteCommon::textureBuffers_;

SpriteCommon::~SpriteCommon()
{

}


template <typename Type1, typename Type2, typename Type3>
void SpriteCommon::CrateConstBuff(Type1*& constBuffer, Type2*& constMapData, Type3* directXBasic_)
//Comptr�Ȃ�&�ŎQ��
//*&�ŎQ��
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
	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer));
	assert(SUCCEEDED(result_));


	//�萔�o�b�t�@�̃}�b�s���O
	result_ = constBuffer->Map(0, nullptr, (void**)&constMapData);//�}�b�s���O
	assert(SUCCEEDED(result_));

}


template <typename Type3, typename Type4, typename Type5>
void SpriteCommon::TestCrateConstBuff(Type3** constBuffer, Type4*& constMapData, Type5* directXBasic_)
//Comptr�Ȃ�&�ŎQ��
//*&�ŎQ��
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
	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer));
	assert(SUCCEEDED(result_));


	//�萔�o�b�t�@�̃}�b�s���O
	result_ = constBuffer->Map(0, nullptr, (void**)&constMapData);//�}�b�s���O
	assert(SUCCEEDED(result_));

}



void SpriteCommon::Initialize(DirectXBasic* directXBasic)
{
	directXBasic_ = directXBasic;

	//ID3D12Resource* constBuffMaterial_ = constBuffMaterial.Get();

	////SpriteCommon::CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial.Get(), constMapMaterial, directXBasic_);
	//CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial_, constMapMaterial, directXBasic_);

	ID3D12Resource* constBuffMaterial_ = constBuffMaterial.Get();
	ID3D12Resource* constBuffTransform_ = constBuffTransform.Get();

	Microsoft::WRL::ComPtr<ID3D12Resource> testconstBuffMaterial_;
	Microsoft::WRL::ComPtr<ID3D12Resource> testconstBuffTransform_;

	CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial_, constMapMaterial, directXBasic_);
	CrateConstBuff<ID3D12Resource, ConstBufferDataTransform, DirectXBasic>(constBuffTransform_, constMapTransform, directXBasic_);

	//TestCrateConstBuff<Microsoft::WRL::ComPtr<ID3D12Resource>, ConstBufferDataTransform, DirectXBasic>(testconstBuffMaterial_.GetAddressOf(), constMapMaterial, directXBasic_);
	
	//CrateConstBuff<ID3D12Resource, ConstBufferDataMaterial, DirectXBasic>(constBuffMaterial_.GetAddressOf(), constMapMaterial, directXBasic_);
	//CrateConstBuff<ID3D12Resource, ConstBufferDataTransform, DirectXBasic>(constBuffTransform_.GetAddressOf(), constMapTransform, directXBasic_);

	constBuffMaterial = constBuffMaterial_;
	constBuffTransform = constBuffTransform_;

	
	//�P�ʍs�����
	constMapTransform->mat = XMMatrixIdentity();
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXBasic_->GetWinWidth();		//�E�C���h�E����
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXBasic_->GetWinHeight();	//�E�C���h�E�c��
	//��ʔ����̕��s�ړ�
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	HRESULT result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result_));

	/*delete constBuffMaterial_;
	delete constBuffTransform_;*/

}


void SpriteCommon::CrateConstBuffTransform()
{
	//CrateConstBuff<ID3D12Resource*, ConstBufferDataTransform*, DirectXBasic*>(constBuffTransform.Get(), constMapTransform, directXBasic_);
	//
	////�P�ʍs�����
	//constMapTransform->mat = XMMatrixIdentity();
	//constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXBasic_->GetWinWidth();		//�E�C���h�E����
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXBasic_->GetWinHeight();	//�E�C���h�E�c��
	////��ʔ����̕��s�ړ�
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
}

//�����������̏�����
//void SpriteCommon::Initialize(DirectXBasic* directXBasic)
//{
//	directXBasic_ = directXBasic;
//
//	//�q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC cbResourceDesc{};
//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
//	cbResourceDesc.Height = 1;
//	cbResourceDesc.DepthOrArraySize = 1;
//	cbResourceDesc.MipLevels = 1;
//	cbResourceDesc.SampleDesc.Count = 1;
//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//�萔�o�b�t�@�̐���
//	HRESULT result_ = directXBasic_->GetDevice()->CreateCommittedResource(
//		&cbHeapProp,//�q�[�v�ݒ�
//		D3D12_HEAP_FLAG_NONE,
//		&cbResourceDesc,//���\�[�X�ݒ�
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffMaterial));
//	assert(SUCCEEDED(result_));
//
//	//�萔�o�b�t�@�̃}�b�s���O
//	result_ = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
//	assert(SUCCEEDED(result_));
//
//
//	//�֐�������܂ł̉��}���u
//	{
//		//�q�[�v�ݒ�
//		D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
//		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//		//���\�[�X�ݒ�
//		D3D12_RESOURCE_DESC cbResourceDesc{};
//		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
//		cbResourceDesc.Height = 1;
//		cbResourceDesc.DepthOrArraySize = 1;
//		cbResourceDesc.MipLevels = 1;
//		cbResourceDesc.SampleDesc.Count = 1;
//		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//		//�萔�o�b�t�@�̐���
//		result_ = directXBasic_->GetDevice()->CreateCommittedResource(
//			&cbHeapProp,//�q�[�v�ݒ�
//			D3D12_HEAP_FLAG_NONE,
//			&cbResourceDesc,//���\�[�X�ݒ�
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&constBuffTransform));
//		assert(SUCCEEDED(result_));
//
//		//�萔�o�b�t�@�̃}�b�s���O
//		result_ = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//�}�b�s���O
//		assert(SUCCEEDED(result_));
//
//		//�P�ʍs�����
//		constMapTransform->mat = XMMatrixIdentity();
//		constMapTransform->mat.r[0].m128_f32[0] = 2.0f / directXBasic_->GetWinWidth();		//�E�C���h�E����
//		constMapTransform->mat.r[1].m128_f32[1] = -2.0f / directXBasic_->GetWinHeight();	//�E�C���h�E�c��
//		//��ʔ����̕��s�ړ�
//		constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
//		constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
//
//	}
//
//	//�f�X�N���v�^�q�[�v�̐ݒ�
//	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
//	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
//	srvHeapDesc.NumDescriptors = kMaxSRVCount;
//
//	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
//	HRESULT result_ = directXBasic_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
//	assert(SUCCEEDED(result_));
//}



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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

}

//���Z����
void SpriteCommon::Add()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));

}

//���Z����
void SpriteCommon::Sub()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
}

//���]
void SpriteCommon::InvertColor()
{
	//���_���C�A�E�g�̐ݒ�
	VertexLayoutSet();

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	PipelineSet();

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

	RootSignatureSet();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState *pipelineState = nullptr;
	HRESULT result_ = directXBasic_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result_));
}

//�摜�ǂݍ���
void SpriteCommon::LoadTexture(const std::string& fileName)
{

	//�摜�ԍ�
	textureIndex_++;

	//�摜�̕�����Ɖ摜�ԍ����i�[
	textureMap.emplace(fileName, textureIndex_);

	//�f�B���N�g���p�X�ƃt�@�C������A�����𓾂�
	std::string fullPath = kDefaultTextureDirectoryPath_ + fileName;

	//���C�h������ɕϊ������ۂ̕�����o�b�t�@�T�C�Y���v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, nullptr, 0);

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
		IID_PPV_ARGS(&textureBuffers_[textureIndex_]));

	/*SetWidth(textureResourceDesc.Width);
	SetHeight(textureResourceDesc.Height);*/

	//�S�~�j�}�b�v�ɂ���
	for(size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��

		result_ = textureBuffers_[textureIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result_));
	}


	//----------------------------------

	////�q�[�v�ݒ�
	//D3D12_HEAP_PROPERTIES textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

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
	//	D3D12_RESOURCE_STATE_COPY_DEST,
	//	nullptr,
	//	IID_PPV_ARGS(&textureBuffers_[textureIndex_]));

	////���C�A�E�g�擾
	//D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint;
	//UINT64  total_bytes = 0;
	//directXBasic_->GetDevice()->GetCopyableFootprints(&textureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	////�]���p�̃o�b�t�@���쐬
	////�]���p�q�[�v�ݒ�
	//D3D12_HEAP_PROPERTIES  uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	////�]���p���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC  uploadDesc;
	//memset(&uploadDesc, 0, sizeof(uploadDesc));
	//uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//uploadDesc.Width = total_bytes; // ��
	//uploadDesc.Height = 1; // ��
	//uploadDesc.DepthOrArraySize = 1;
	//uploadDesc.MipLevels = 1;
	//uploadDesc.SampleDesc.Count = 1;
	//uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	////�]���p�e�N�X�`���o�b�t�@�̐���
	//ID3D12Resource* uploadBuffer = nullptr;
	//result_ = directXBasic_->GetDevice()->CreateCommittedResource(&uploadHeap,
	//	D3D12_HEAP_FLAG_NONE, &uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));

	////�]���p�o�b�t�@�ւ̏�������
	///*void* ptr = nullptr;
	//uploadBuffer->Map(0, nullptr, &ptr);*/
	////for(size_t i = 0; i < metadata.mipLevels; i++)
	////{
	////	//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
	////	//const Image* img = scratchImg.GetImage(i, 0, 0);
	////	/*const uint8_t* pixel[] = { 0 };
	////	pixel[i] = scratchImg.GetPixels();*/
	////	const uint8_t* pixel = scratchImg.GetImage(i,0,0)->pixels;

	////	/*uint8_t pixelData[];
	////	size_t pixelDataSize = scratchImg.GetPixelsSize();*/

	////	//memcpy(reinterpret_cast<unsigned char*>(ptr) + footprint.Offset, img->pixels, scratchImg.GetPixelsSize());
	////	
	////}
	////�S�~�j�}�b�v�ɂ���
	////for(size_t i = 0; i < metadata.mipLevels; i++)
	////{
	////	//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
	////	const Image* img = scratchImg.GetImage(i, 0, 0);

	////	result_ = uploadBuffer->Map(
	////				(UINT)i,
	////				nullptr,
	////				&ptr);

	////	memcpy(reinterpret_cast<unsigned char*>(ptr) + footprint.Offset, img->pixels, sizeof(img->pixels));

	////	//textureBuffers_[textureIndex_]->
	////}

	////�S�~�j�}�b�v�ɂ���
	//for(size_t i = 0; i < metadata.mipLevels; i++)
	//{
	//	//�~�j�}�b�v���x�����w�肵�ăC���[�W���擾
	//	const Image* img = scratchImg.GetImage(i, 0, 0);

	//	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��

	//	result_ = uploadBuffer->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,
	//		img->pixels,
	//		(UINT)img->rowPitch,
	//		(UINT)img->slicePitch
	//	);

	//	assert(SUCCEEDED(result_));
	//}

	//
	////�R�s�[�R�}���h�쐬
	//D3D12_TEXTURE_COPY_LOCATION  dest;
	//memset(&dest, 0, sizeof(dest));
	//dest.pResource = textureBuffers_[textureIndex_].Get();
	//dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	//dest.SubresourceIndex = 0;

	//D3D12_TEXTURE_COPY_LOCATION  src;
	//memset(&src, 0, sizeof(src));
	//src.pResource = uploadBuffer;
	//src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	//src.PlacedFootprint = footprint;

	////�]���R�}���h���i�[
	//ID3D12GraphicsCommandList* iCommandList = directXBasic_->GetCommandList().Get();
	//iCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

	////�R�}���h�L���[�ƃR�}���h���X�g���`��p�Ƌ��p�Ȃ̂Ń��\�[�X�o���A���K�v
	////���\�[�X�o���A�̐ݒ�
	//D3D12_RESOURCE_BARRIER  barrier;
	//memset(&barrier, 0, sizeof(barrier));
	//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//barrier.Transition.pResource = textureBuffers_[textureIndex_].Get();
	//barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	////���\�[�X�o���A�̐���
	//iCommandList->ResourceBarrier(1, &barrier);

	//----------------------------

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	//�f�X�N���v�^�̃T�C�Y���擾
	UINT incrementSize = directXBasic_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�擾�����T�C�Y���g�p���ăn���h����i�߂�
	for(uint32_t i = 0; i < textureIndex_; i++)
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
	directXBasic_->GetDevice()->CreateShaderResourceView(textureBuffers_[textureIndex_].Get(), &srvDesc, srvHandle);
}

void SpriteCommon::Update()
{
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
}

void SpriteCommon::VertexLayoutSet()
{
	// ���_���C�A�E�g
	inputLayout =
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

		// uv���W
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//���W�ȊO�ɁA�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};
}

void SpriteCommon::PipelineSet()
{
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

#pragma region �u�����h�ݒ�(03_01)

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = &inputLayout.at(0);
	//pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout.size());

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

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
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));
	result_ = directXBasic_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result_));
}

void SpriteCommon::DescriptorHeapSet()
{

	////SRV�q�[�v�̐ݒ�R�}���h
	//directXBasic_->GetCommandList()->SetDescriptorHeaps(1, &srvHeap_);

	////GPU��SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	//srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

}

void SpriteCommon::BeforeDraw()
{
	//�p�C�v���C���̃Z�b�g
	directXBasic_->GetCommandList()->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̃Z�b�g
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�g�|���W�[�̃Z�b�g
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�f�X�N���v�^�q�[�v�̃Z�b�g
	DescriptorHeapSet();
}

void SpriteCommon::AfterDraw() {}

