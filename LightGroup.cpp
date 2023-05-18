#include "LightGroup.h"

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device *device)
{
	//�ď������`�F�b�N
	assert(!LightGroup::device);

	//nullptr�`�F�b�N
	assert(device);

	LightGroup::device = device;
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
	HRESULT result = device->CreateCommittedResource(
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
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LightGroup *instance = new LightGroup();

	//������
	instance->Initialize();

	return instance;
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
		for(int i = 0; i < DirLightNum; i++)
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

		//�_����
		for(int i = 0; i < PointLightNum; i++)
		{
			//���C�g���L���Ȃ�ݒ��]��
			//if(pointLights[i].IsActive())
			//{
			//	constMap->pointLights[i].active = 1;
			//	constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
			//	constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
			//	constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
			//}
			////���C�g�������Ȃ烉�C�g�F��0��
			//else
			//{
			//	constMap->pointLights[i].active = 0;
			//}
		}

		//�X�|�b�g���C�g
		for(int i = 0; i < SpotLightNum; i++)
		{
			//���C�g���L���Ȃ�ݒ��]��
			//if(spotLights[i].IsActive())
			//{
			//	constMap->spotLights[i].active = 1;
			//	constMap->spotLights[i].lightv = -spotLights[i].GetLightDir();
			//	constMap->spotLights[i].lightPos = spotLights[i].GetLightPos();
			//	constMap->spotLights[i].lightColor = spotLights[i].GetLightColor();
			//	constMap->spotLights[i].lightAtten = spotLights[i].GetLightAtten();
			//	constMap->spotLights[i].lightFactorAngleCos = spotLights[i].GetLightFactorAngleCos();
			//}
			////���C�g�������Ȃ烉�C�g�F��0��
			//else
			//{
			//	constMap->spotLights[i].active = 0;
			//}
		}

		//�ۉe
		for(int i = 0; i < CircleShadowNum; i++)
		{
			//�L���Ȃ�ݒ��]��
			//if(circleShadows[i].IsActive())
			//{
			//	constMap->circleShadows[i].active = 1;
			//	constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
			//	constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
			//	constMap->circleShadows[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
			//	constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
			//	constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
			//}
			////�����Ȃ�F��0��
			//else
			//{
			//	constMap->circleShadows[i].active = 0;
			//}
		}

		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const XMFLOAT3 &color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR &lightDir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights_[index].SetLightDir(lightDir);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3 &lightColor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights_[index].SetLightColor(lightColor);
	dirty_ = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[0].SetLightDir({0.0f,-1.0f,0.0f,0});

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[1].SetLightDir({0.5f,0.1f,0.2f,0});

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[2].SetLightDir({ -0.5f,0.1f,-0.2f,0});	
}
//
//void LightGroup::SetPointLightActive(int index, bool active)
//{
//	assert(0 <= index && index < PointLightNum);
//
//	pointLights[index].SetActive(active);
//}
//
//void LightGroup::SetPointLightPos(int index, const XMFLOAT3 &lightPos)
//{
//	assert(0 <= index && index < PointLightNum);
//
//	pointLights[index].SetLightPos(lightPos);
//	dirty_ = true;
//}
//
//void LightGroup::SetPointLightColor(int index, const XMFLOAT3 &lightColor)
//{
//	assert(0 <= index && index < PointLightNum);
//	
//	pointLights[index].SetLightColor(lightColor);
//	dirty_ = true;
//}
//
//void LightGroup::SetPointLightAtten(int index, const XMFLOAT3 &lightAtten)
//{
//	assert(0 <= index && index < PointLightNum);
//
//	pointLights[index].SetLightAtten(lightAtten);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightActive(int index, bool active)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetActive(active);
//}
//
//void LightGroup::SetSpotLightDir(int index, const XMVECTOR &lightDir)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightDir(lightDir);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightPos(int index, const XMFLOAT3 &lightPos)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightPos(lightPos);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightColor(int index, const XMFLOAT3 &lightColor)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightColor(lightColor);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightAtten(int index, const XMFLOAT3 &lightAtten)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightAtten(lightAtten);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightFactorAngle(int index, const XMFLOAT2 &lightFactorAngle)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightFactorAngle(lightFactorAngle);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowActive(int index, bool active)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetActive(active);
//}
//
//void LightGroup::SetCircleShadowCasterPos(int index, const XMFLOAT3 &casterPos)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetCasterPos(casterPos);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowDir(int index, const XMVECTOR &lightDir)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetDir(lightDir);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowDistanceCasterLight(int index, float &distanceCasterLight)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowAtten(int index, const XMFLOAT3 &lightAtten)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetAtten(lightAtten);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowFactorAngle(int index, const XMFLOAT2 &lightFactorAngle)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetFactorAngle(lightFactorAngle);
//	dirty_ = true;
//}
