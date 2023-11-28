#pragma once
#include "DirectXBasic.h"
#include "Vector2.h"
#include "Vector4.h"
#include <wrl.h>
#include <array>
#include <map>

class TextureManager final
{
private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	TextureManager();
	~TextureManager();

public:

	static TextureManager* GetInstance()
	{
		static TextureManager textureManager;
		return &textureManager;
	}

	//静的初期化
	static void StaticInitialize(ID3D12Device* device);

	//初期化
	void Initialize();

	//テクスチャ読み込み
	static void LoadTexture(const std::string& fileName);

	//テクスチャ生成
	void TexMapping(int32_t texWidth, int32_t texHeight, Vector4 color, const std::string& fileName);

public:

	//SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;

private:

	//シェーダーリソース用のデスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> srvHeap_;

	//テクスチャバッファ
	static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> textureBuffers_;

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	//テクスチャ番号
	static uint32_t sTextureIndex_;
	//画像に結び付いたテクスチャ番号格納用map
	static std::map<const std::string, uint32_t, std::less<>> textureMap_;

	//テクスチャの幅
	static std::map<const std::string, Vector2> texSizeMap_;

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* cmdList_;

public:

	//コピーコンストラクタを無効
	TextureManager(const TextureManager& textureManager) = delete;
	//代入演算子を無効
	TextureManager& operator= (const TextureManager& textureManager) = delete;

public:

	//ゲッター
	ID3D12DescriptorHeap* GetSRVHeap() const { return srvHeap_.Get(); };
	const std::map<const std::string, uint32_t, std::less<>>& GetTextureMap() const { return textureMap_; }
	Vector2 GetTexSize(std::string fileName);
};

