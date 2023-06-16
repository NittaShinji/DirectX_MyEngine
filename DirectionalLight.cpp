//#include "DirectionalLight.h"
//using namespace DirectX;
//
/////<summary>
////静的メンバ変数の実体
/////<summary>
//ID3D12Device*DirectionalLight::device = nullptr;
//
//DirectionalLight *DirectionalLight::Create()
//{
//	//3Dオブジェクトのインスタンスを生成
//	DirectionalLight * instance = new DirectionalLight();
//	//初期化
//	instance->Initialize();
//	//生成したインスタンスを返す
//	return instance;
//}
//
//void DirectionalLight::StaticInitialize(ID3D12Device* device)
//{
//	//再初期化チェック
//	assert(!DirectionalLight::device);
//	//nullptrチェック
//	assert(device);
//	//静的メンバ変数のセット
//	DirectionalLight::device = device;
//}
//
//void DirectionalLight::Initialize()
//{
//	//ヒープ設定
//	D3D12_HEAP_PROPERTIES cbHeapProp{};				//GPUへの転送用
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//リソース設定
//	D3D12_RESOURCE_DESC cbResourceDesc{};
//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;	//256バイトアラインメント
//	cbResourceDesc.Height = 1;
//	cbResourceDesc.DepthOrArraySize = 1;
//	cbResourceDesc.MipLevels = 1;
//	cbResourceDesc.SampleDesc.Count = 1;
//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	///定数バッファの生成
//	HRESULT result = device->CreateCommittedResource(
//		&cbHeapProp,//ヒープ設定
//		D3D12_HEAP_FLAG_NONE,
//		&cbResourceDesc,//リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuff));
//	assert(SUCCEEDED(result));
//
//	//定数バッファへデータ転送
//	TransferConstBuffer();
//}
//
//
//void DirectionalLight::Update()
//{
//	//値の更新があった時だけ定数バッファに転送する
//	if (dirty)
//	{
//		TransferConstBuffer();
//		dirty = false;
//	}
//}
//
//void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdlist, UINT rootParameterIndex)
//{
//	//定数バッファビューをセット
//	cmdlist->SetGraphicsRootConstantBufferView(rootParameterIndex, 
//		constBuff->GetGPUVirtualAddress());
//}
//
//void DirectionalLight::TransferConstBuffer()
//{
//	HRESULT result;
//	//定数バッファへデータ転送
//	ConstBufferData* constMap = nullptr;
//	result = constBuff->Map(0, nullptr, (void**)&constMap);
//	if (SUCCEEDED(result))
//	{
//		constMap->lightv = -lightDir;
//		constMap->lightColor = lightColor;
//		constBuff->Unmap(0, nullptr);
//	}
//}
//
//void DirectionalLight::SetLightDir(const XMVECTOR& lightDir)
//{
//	//正規化してセット
//	this->lightDir = XMVector3Normalize(lightDir);
//	dirty = true;
//}
//
//void DirectionalLight::SetLightColor(const XMFLOAT3& lightColor)
//{
//	this->lightColor = lightColor;
//	dirty = true;
//}
