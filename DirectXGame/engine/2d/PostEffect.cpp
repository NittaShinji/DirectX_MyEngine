#include "PostEffect.h"
#include "WindowsAPI.h"
#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect() 
	: Sprite()
{
}

void PostEffect::Initialize()
{
	HRESULT result;

	//���N���X�Ƃ��Ă̏�����
	Sprite::Initialize(XMFLOAT2(0, 0), XMFLOAT2(170, 170));

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowsAPI::kWindow_width_,
		(UINT)WindowsAPI::kWindow_height_,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	D3D12_HEAP_PROPERTIES textureHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	//�e�N�X�`���o�b�t�@�̐���
	result = directXBasic_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
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


}

void PostEffect::Draw(const std::string& fileName)
{
	matUpdate();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	directXBasic_->GetCommandList()->SetPipelineState(spriteCommon_->GetPipelineState().Get());
	//���[�g�V�O�l�`���̐ݒ�
	directXBasic_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature_().Get());
	//�v���~�e�B�u�`���ݒ�
	directXBasic_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	spriteCommon_->Update();
	
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directXBasic_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//SRV�q�[�v�̐ݒ�R�}���h
	//ID3D12DescriptorHeap* heaps[] = { spriteCommon_->GetSRVHeap() };
	ID3D12DescriptorHeap* heaps[] = { descHeapSRV.Get()};
	directXBasic_->GetCommandList()->SetDescriptorHeaps(1, heaps);

	//GPU��SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spriteCommon_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

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
	directXBasic_->GetCommandList()->SetGraphicsRootConstantBufferView(2, spriteCommon_->GetConstBuffTransform()->GetGPUVirtualAddress());

	if(isInvisible_)
	{
		return;
	}

	//�`��R�}���h(���_���A�C���X�^���X�̐��A�ŏ��̒��_�̃C���f�b�N�X,�f�[�^��ǂݎ��O�Ɋe�C���f�b�N�X�ɒǉ������l)
	directXBasic_->GetCommandList()->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);
}
