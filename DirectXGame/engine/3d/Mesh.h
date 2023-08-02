#pragma once
#include "DirectXBasic.h"
#include "Vector2.h"
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>

class Mesh
{

private:

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectXを省略
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	//using Vector2 = DirectX::Vector2;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//頂点情報
	struct Vertex
	{
		XMFLOAT3 pos;		// xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		Vector2 uv;		// uv座標
	};

public:

	static void StaticInitialize(DirectXBasic* directXbasic);

	//頂点情報をセット
	void SetVertices(const Vertex& vertex);

	//頂点インデックスをセット
	void SetIndices(const unsigned short& index);

	void CrateBuffer();

	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<Vertex>& GetVertices() { return vertices_; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

	/// <summary>
	/// 頂点バッファ取得
	/// </summary>
	/// <returns>頂点バッファ</returns>
	inline const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }

	/// <summary>
	/// インデックスバッファ取得
	/// </summary>
	/// <returns>インデックスバッファ</returns>
	inline const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }

private:

	static DirectXBasic* directXBasic_;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	//頂点データ配列
	std::vector<Vertex> vertices_;
	//頂点インデックス配列
	std::vector<unsigned short> indices_;

};

