#include "LightGroup.h"

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* LightGroup::device_ = nullptr;

void LightGroup::StaticInitialize(ID3D12Device *device)
{
	//�ď������`�F�b�N
	assert(!LightGroup::device_);

	//nullptr�`�F�b�N
	assert(device);

	LightGroup::device_ = device;
}

void LightGroup::Initialize()
{
	//�W���̃��C�g�̐ݒ�
	DefaultLightSetting();

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
	HRESULT result = device_->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void LightGroup::Update()
{
	//�l�̍X�V���������Ƃ������萔�o�b�t�@�ɓ]������
	if(dirty_ == true)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

LightGroup *LightGroup::Create()
{
	//3D�I�u�W�F�N�g�̐ÓI�C���X�^���X�𐶐�
	static LightGroup instance;
	instance.Initialize();

	return &instance;
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData *constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void **)&constMap);
	if(SUCCEEDED(result))
	{
		//����
		constMap->ambientColor = ambientColor_;
		
		//���s����
		for(int32_t i = 0; i < kDirLightNum_; i++)
		{
			//���C�g���L���Ȃ�ݒ��]��
			if(dirLights_[i].IsActive())
			{
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights_[i].GetLightColor();

			}
			//���C�g�������Ȃ�]�����Ȃ�
			else
			{
				constMap->dirLights[i].active = 0;
			}
		}

		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const Vector3 &color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int32_t index, const Vector3&lightDir,const float upVec)

{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetLightDir(lightDir,upVec);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(int32_t index, const Vector3 &lightColor)
{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetLightColor(lightColor);
	dirty_ = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[0].SetLightDir(Vector3{ 0.0f,-1.0f,0.0f }, 0.0f);

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[1].SetLightDir(Vector3{0.5f,0.1f,0.2f},0.0f);

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[2].SetLightDir(Vector3{-0.5f,0.1f,-0.2f},0.0f);
}
