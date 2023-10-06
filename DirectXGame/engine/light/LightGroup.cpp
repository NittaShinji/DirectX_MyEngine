#include "LightGroup.h"

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* LightGroup::device_ = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
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

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

LightGroup* LightGroup::Create()
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
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
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

		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(int32_t index, bool active)
{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int32_t index, const Vector3& lightDir, const float upVec)

{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetLightDir(lightDir, upVec);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(int32_t index, const Vector3& lightColor)
{
	assert(0 <= index && index < kDirLightNum_);
	dirLights_[index].SetLightColor(lightColor);
	dirty_ = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[0].SetLightDir(Vector3{ 0.0f,-1.0f,0.0f }, 0.0f);

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[1].SetLightDir(Vector3{ 0.5f,0.1f,0.2f }, 0.0f);

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[2].SetLightDir(Vector3{ -0.5f,0.1f,-0.2f }, 0.0f);
}
