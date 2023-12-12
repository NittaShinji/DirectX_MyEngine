#include "TextureManager.h"
#include <d3dcompiler.h>
#include <string.h>
#pragma warning(push)
#pragma warning(disable:4061)
#pragma warning(disable:4820)
#include <DirectXTex.h>
#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//シェーダーリソース用のデスクリプタヒープ
ComPtr<ID3D12DescriptorHeap> TextureManager::srvHeap_;
//SRVの最大個数
const size_t kMaxSRVCount_ = 2056;
//テクスチャバッファ
std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> TextureManager::textureBuffers_;
//デフォルトテクスチャ格納ディレクトリ
std::string TextureManager::kDefaultTextureDirectoryPath_ = "Resources/Sprite/";
//テクスチャ番号
uint32_t TextureManager::sTextureIndex_;
//画像に結び付いたテクスチャ番号格納用map
std::map<const std::string, uint32_t, std::less<>> TextureManager::textureMap_;
std::map<const std::string, Vector2> TextureManager::texSizeMap_;

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
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount_;

	//設定を本にSRV用デスクリプタヒープを生成
	HRESULT result;
	result = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	texConverter_ = std::make_unique<TextureConverter>();
}

void TextureManager::LoadTexture(const std::string& fileName)
{
	HRESULT result;

	//画像の文字列と画像番号を格納
	textureMap_.emplace(fileName, sTextureIndex_);

	//ディレクトリパスとファイル名を連結しを得る
	std::string fullPath = kDefaultTextureDirectoryPath_ + fileName;

	//画像ファイルの用意
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	size_t pos1;
	std::wstring wfilePath = ConvertMultiByteStringToWideString(fullPath);
	std::wstring exceptExt;
	std::wstring fileExt;

	//区切り文字 '.'が出てくる一番最後の部分を検索
	pos1 = wfilePath.rfind('.');
	//検索がヒットしたら
	if(pos1 != std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		fileExt = wfilePath.substr(pos1 + 1, wfilePath.size() - pos1 - 1);
		//区切り文字の前までを抜き出す
		exceptExt = wfilePath.substr(0, pos1);
	}
	else
	{
		fileExt = L"";
		exceptExt = wfilePath;
	}

	//拡張子のマルチバイト文字列に変換
	std::string stringExt = ConvertWideStringToMultiByteString(fileExt);
	std::erase(stringExt, '\0');
	
	if(stringExt == "dds")
	{
		//DDSテクスチャのロード
		result = LoadFromDDSFile(wfilePath.c_str(), DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else if(stringExt == "png")
	{
		//WICテクスチャのロード
		result = LoadFromWICFile(wfilePath.data(), WIC_FLAGS_NONE, &metadata, scratchImg);
	}

	//ミニマップ生成
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if(SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width; // 横幅
	textureResourceDesc.Height = (UINT)metadata.height; // 縦幅
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	result = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffers_[sTextureIndex_]));

	//全ミニマップについて
	for(size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミニマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		//テクスチャバッファにデータ転送

		result = textureBuffers_[sTextureIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);

		assert(SUCCEEDED(result));
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	for(uint32_t i = 0; i < sTextureIndex_; i++)
	{
		srvHandle.ptr += incrementSize;
	}

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device_->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);

	//テクスチャのサイズを保存
	texSizeMap_.emplace(fileName, Vector2(static_cast<float>(textureResourceDesc.Width), static_cast<float>(textureResourceDesc.Height)));

	//画像番号を増やす
	sTextureIndex_++;
}

void TextureManager::TexMapping(int32_t texWidth, int32_t texHeight, Vector4 color, const std::string& fileName)
{
	//画像の文字列と画像番号を格納
	textureMap_.emplace(fileName, sTextureIndex_);

	//画像イメージデータ配列
	Vector4* imageDate;
	//配列の要素数
	const int32_t imageDateCount_ = texWidth * texHeight;
	//画像イメージデータ配列
	imageDate = new Vector4[imageDateCount_];

	//テクスチャのサイズを保存
	texSizeMap_.emplace(fileName, Vector2(static_cast<float>(texWidth), static_cast<float>(texHeight)));

	//全ピクセルの色を初期化
	for(size_t i = 0; i < imageDateCount_; i++)
	{
		imageDate[i].x = color.x; // R
		imageDate[i].y = color.y; // G
		imageDate[i].z = color.z; // B
		imageDate[i].w = color.w; // A
	}

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = texWidth; // 幅
	textureResourceDesc.Height = texHeight; // 幅
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	HRESULT result_ = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffers_[sTextureIndex_]));

	//テクスチャバッファにデータ転送
	result_ = textureBuffers_[sTextureIndex_]->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		imageDate,	//元データアドレス
		sizeof(Vector4) * texWidth,	// 1ラインサイズ
		sizeof(Vector4) * imageDateCount_	//　全サイズ
	);

	delete[] imageDate;

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();

	//デスクリプタのサイズを取得
	UINT incrementSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//取得したサイズを使用してハンドルを進める
	for(uint32_t i = 0; i < sTextureIndex_; i++)
	{
		srvHandle.ptr += incrementSize;
	}

	//シェーダーリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device_->CreateShaderResourceView(textureBuffers_[sTextureIndex_].Get(), &srvDesc, srvHandle);

	//画像番号を増やす
	sTextureIndex_++;
}

void TextureManager::TexConvert(std::string fileName)
{
	//ディレクトリパスとファイル名を連結しを得る
	std::string fullPath = kDefaultTextureDirectoryPath_ + fileName;

	//コンバーターで変換
	texConverter_->ConvertTextureWICToDDS(fullPath);
}

std::string TextureManager::ConvertWideStringToMultiByteString(const std::wstring& wstring)
{
	int32_t length = static_cast<int32_t>(wstring.length());
	//マルチバイト文字列に変換した際の文字列バッファサイズを計算
	int32_t multiBufferSize = WideCharToMultiByte(CP_ACP, 0, wstring.c_str(), length, 0, 0, 0, 0);
	std::string resultString(multiBufferSize, '\0');
	//マルチバイト文字列に変換
	if(multiBufferSize) WideCharToMultiByte(CP_ACP, 0, wstring.c_str(), length, &resultString[0], multiBufferSize, 0, 0);

	return resultString;
}

std::wstring TextureManager::ConvertMultiByteStringToWideString(const std::string& mString)
{
	//ワイド文字列に変換した際の文字列バッファサイズを計算
	int32_t filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//ワイド文字列に変換
	std::wstring wString;
	wString.resize(filePathBufferSize);

	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

Vector2 TextureManager::GetTexSize(std::string fileName)
{
	Vector2 result;
	result = texSizeMap_.at(fileName);
	return result;
}

ID3D12Resource* TextureManager::GetTextureBuffer(std::string fileName)
{
	uint32_t targetNumber;
	targetNumber = textureMap_.at(fileName);

	ID3D12Resource* result = nullptr;
	result = textureBuffers_[targetNumber].Get();

	return result;
}
