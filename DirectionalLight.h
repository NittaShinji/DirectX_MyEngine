#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>

class DirectionalLight
{
private: //エイリアステンプレート

	//Microsoft::WRL::を省略
	template<class T>using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス

	//定数バッファ用データ構造体
	struct  ConstBufferData
	{
		XMVECTOR lightv;		//ライトへ方向を表すベクトル
		XMFLOAT3 lightColor;	//ライトの色
		bool active;	//有効フラグ
	};

public: //アクセッサ

	//ライト方向をセット
	void SetLightDir(const XMVECTOR& lightDir) 
	{
		//正規化してセット
		this->lightDir = DirectX::XMVector3Normalize(lightDir);
		dirty = true;
	}

	//ライト色のセット
	void SetLightColor(const XMFLOAT3& lightColor)
	{
		this->lightColor = lightColor;
		dirty = true;
	}

	XMVECTOR GetLightDir() { return lightDir; };
	XMFLOAT3 GetLightColor() { return lightColor; };

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active) { this->active = active; }

	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active; }

private: //メンバ変数

	//定数バッファ
	Comptr<ID3D12Resource> constBuff;
	//ライト光線方向(単位ベクトル)
	XMVECTOR lightDir = { 1,0,0,0};
	//ライト色
	XMFLOAT3 lightColor = { 1,1,1 };
	//有効フラグ
	bool active = false;
	//ダーティフラグ(値に変更があったときだけ定数バッファに転送)
	bool dirty = false;
};
