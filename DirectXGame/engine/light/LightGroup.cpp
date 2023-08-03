#include "LightGroup.h"

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* LightGroup::device_ = nullptr;

void LightGroup::StaticInitialize(ID3D12Device *device)
{
	//再初期化チェック
	assert(!LightGroup::device_);

	//nullptrチェック
	assert(device);

	LightGroup::device_ = device;
}

void LightGroup::Initialize()
{
	//標準のライトの設定
	DefaultLightSetting();

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	///定数バッファの生成
	HRESULT result = device_->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void LightGroup::Update()
{
	//値の更新があったときだけ定数バッファに転送する
	if(dirty_ == true)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

LightGroup *LightGroup::Create()
{
	//3Dオブジェクトの静的インスタンスを生成
	static LightGroup instance;
	instance.Initialize();

	return &instance;
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void **)&constMap);
	if(SUCCEEDED(result))
	{
		//環境光
		constMap->ambientColor = ambientColor_;
		
		//平行光源
		for(int32_t i = 0; i < kDirLightNum_; i++)
		{
			//ライトが有効なら設定を転送
			if(dirLights_[i].IsActive())
			{
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights_[i].GetLightColor();

			}
			//ライトが無効なら転送しない
			else
			{
				constMap->dirLights[i].active = 0;
			}
		}

		//点光源
		for(int32_t i = 0; i < kPointLightNum_; i++)
		{
			//ライトが有効なら設定を転送
			//if(pointLights[i].IsActive())
			//{
			//	constMap->pointLights[i].active = 1;
			//	constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
			//	constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
			//	constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
			//}
			////ライトが無効ならライト色を0に
			//else
			//{
			//	constMap->pointLights[i].active = 0;
			//}
		}

		//スポットライト
		for(int32_t i = 0; i < kSpotLightNum_; i++)
		{
			//ライトが有効なら設定を転送
			//if(spotLights[i].IsActive())
			//{
			//	constMap->spotLights[i].active = 1;
			//	constMap->spotLights[i].lightv = -spotLights[i].GetLightDir();
			//	constMap->spotLights[i].lightPos = spotLights[i].GetLightPos();
			//	constMap->spotLights[i].lightColor = spotLights[i].GetLightColor();
			//	constMap->spotLights[i].lightAtten = spotLights[i].GetLightAtten();
			//	constMap->spotLights[i].lightFactorAngleCos = spotLights[i].GetLightFactorAngleCos();
			//}
			////ライトが無効ならライト色を0に
			//else
			//{
			//	constMap->spotLights[i].active = 0;
			//}
		}

		//丸影
		for(int32_t i = 0; i < kCircleShadowNum_; i++)
		{
			//有効なら設定を転送
			//if(circleShadows[i].IsActive())
			//{
			//	constMap->circleShadows[i].active = 1;
			//	constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
			//	constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
			//	constMap->circleShadows[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
			//	constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
			//	constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
			//}
			////無効なら色を0に
			//else
			//{
			//	constMap->circleShadows[i].active = 0;
			//}
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

void LightGroup::SetDirLightDir(int32_t index, const XMVECTOR &lightDir)
{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetLightDir(lightDir);
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
	dirLights_[0].SetLightDir({0.0f,-1.0f,0.0f,0});

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[1].SetLightDir({0.5f,0.1f,0.2f,0});

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[2].SetLightDir({ -0.5f,0.1f,-0.2f,0});	
}

//void LightGroup::SetPointLightActive(int32_t index, bool active)
//{
//	assert(0 <= index && index < kPointLightNum_);
//
//	pointLights[index].SetActive(active);
//}
//
//void LightGroup::SetPointLightPos(int32_t index, const Vector3 &lightPos)
//{
//	assert(0 <= index && index < kPointLightNum_);
//
//	pointLights[index].SetLightPos(lightPos);
//	dirty_ = true;
//}
//
//void LightGroup::SetPointLightColor(int32_t index, const Vector3 &lightColor)
//{
//	assert(0 <= index && index < kPointLightNum_);
//	
//	pointLights[index].SetLightColor(lightColor);
//	dirty_ = true;
//}
//
//void LightGroup::SetPointLightAtten(int32_t index, const Vector3 &lightAtten)
//{
//	assert(0 <= index && index < kPointLightNum_);
//
//	pointLights[index].SetLightAtten(lightAtten);
//	dirty_ = true;
//}

//void LightGroup::SetSpotLightActive(int32_t index, bool active)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetActive(active);
//}
//
//void LightGroup::SetSpotLightDir(int32_t index, const XMVECTOR &lightDir)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightDir(lightDir);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightPos(int32_t index, const Vector3 &lightPos)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightPos(lightPos);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightColor(int32_t index, const Vector3 &lightColor)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightColor(lightColor);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightAtten(int32_t index, const Vector3 &lightAtten)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightAtten(lightAtten);
//	dirty_ = true;
//}
//
//void LightGroup::SetSpotLightFactorAngle(int32_t index, const Vector2 &lightFactorAngle)
//{
//	assert(0 <= index && index < SpotLightNum);
//	spotLights[index].SetLightFactorAngle(lightFactorAngle);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowActive(int32_t index, bool active)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetActive(active);
//}
//
//void LightGroup::SetCircleShadowCasterPos(int32_t index, const Vector3 &casterPos)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetCasterPos(casterPos);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowDir(int32_t index, const XMVECTOR &lightDir)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetDir(lightDir);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowDistanceCasterLight(int32_t index, float &distanceCasterLight)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowAtten(int32_t index, const Vector3 &lightAtten)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetAtten(lightAtten);
//	dirty_ = true;
//}
//
//void LightGroup::SetCircleShadowFactorAngle(int index, const Vector2 &lightFactorAngle)
//{
//	assert(0 <= index && index < CircleShadowNum);
//
//	circleShadows[index].SetFactorAngle(lightFactorAngle);
//	dirty_ = true;
//}
