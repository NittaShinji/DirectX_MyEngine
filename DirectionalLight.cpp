#include "DirectionalLight.h"
using namespace DirectX;

///<summary>
//�ÓI�����o�ϐ��̎���
///<summary>
ID3D12Device*DirectionalLight::device = nullptr;

DirectionalLight *DirectionalLight::Create()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	DirectionalLight * instance = new DirectionalLight();
	//������
	instance->Initialize();
	//���������C���X�^���X��Ԃ�
	return instance;
}

void DirectionalLight::StaticInitialize(ID3D12Device* device)
{
	//�ď������`�F�b�N
	assert(!DirectionalLight::device);
	//nullptr�`�F�b�N
	assert(device);
	//�ÓI�����o�ϐ��̃Z�b�g
	DirectionalLight::device = device;
}

void DirectionalLight::Initialize()
{
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPU�ւ̓]���p
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	///�萔�o�b�t�@�̐���
	HRESULT result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}


void DirectionalLight::Update()
{
	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdlist, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdlist->SetGraphicsRootConstantBufferView(rootParameterIndex, 
		constBuff->GetGPUVirtualAddress());
}

void DirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightDir;
		constMap->lightColor = lightColor;
		constBuff->Unmap(0, nullptr);
	}
}

void DirectionalLight::SetLightDir(const XMVECTOR& lightDir)
{
	//���K�����ăZ�b�g
	this->lightDir = XMVector3Normalize(lightDir);
	dirty = true;
}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightColor)
{
	this->lightColor = lightColor;
	dirty = true;
}
