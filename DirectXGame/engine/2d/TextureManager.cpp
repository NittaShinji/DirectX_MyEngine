#include "TextureManager.h"
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <string.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//�V�F�[�_�[���\�[�X�p�̃f�X�N���v�^�q�[�v
ComPtr<ID3D12DescriptorHeap> TextureManager::srvHeap_;
//SRV�̍ő��
const size_t kMaxSRVCount_ = 2056;
//�e�N�X�`���o�b�t�@
std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> TextureManager::textureBuffers_;
//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
std::string TextureManager::kDefaultTextureDirectoryPath_;
//�e�N�X�`���ԍ�
uint32_t TextureManager::sTextureIndex_;
//�摜�Ɍ��ѕt�����e�N�X�`���ԍ��i�[�pmap
std::map<const std::string, uint32_t, std::less<>> TextureManager::textureMap_;

ID3D12Device* TextureManager::device_;
ID3D12GraphicsCommandList* TextureManager::cmdList_;

TextureManager::TextureManager() {}
TextureManager::~TextureManager() {}

void TextureManager::StaticInitialize(ID3D12Device* device)
{
	TextureManager::device_ = device;
}

void TextureManager::Initialize()
{
	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount_;

	//�ݒ��{��SRV�p�f�X�N���v�^�q�[�v�𐶐�
	HRESULT result_ = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result_));
}

void TextureManager::LoadTexture(const std::string& fileName)
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
	result_ = device_->CreateCommittedResource(
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
	UINT incrementSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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
	device_->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);

	//�摜�ԍ��𑝂₷
	sTextureIndex_++;
}

void TextureManager::TexMapping(int32_t texWidth, int32_t texHeight, Vector4 color, const std::string& fileName)
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
	HRESULT result_ = device_->CreateCommittedResource(
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
	UINT incrementSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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
	device_->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);

	//�摜�ԍ��𑝂₷
	sTextureIndex_++;
}
