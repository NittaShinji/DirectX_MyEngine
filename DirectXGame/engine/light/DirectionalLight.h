#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "Vector3.h"

class DirectionalLight
{
private: //エイリアステンプレート

	//Microsoft::WRL::を省略
	template<class T>using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectXを省略
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス

	//定数バッファ用データ構造体
	struct  ConstBufferData
	{
		XMVECTOR lightv;		//ライトへ方向を表すベクトル
		Vector3 lightColor;	//ライトの色
		bool active;	//有効フラグ
	};

public: //アクセッサ

	//ライト方向をセット
	void SetLightDir(const XMVECTOR& lightDir) 
	{
		//正規化してセット
		this->lightDir_ = DirectX::XMVector3Normalize(lightDir);
		dirty_ = true;
	}

	//ライト色のセット
	void SetLightColor(const Vector3& lightColor)
	{
		this->lightColor_ = lightColor;
		dirty_ = true;
	}

	XMVECTOR GetLightDir() { return lightDir_; };
	Vector3 GetLightColor() { return lightColor_; };

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active) { this->active_ = active; }

	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active_; }

private: //メンバ変数

	//定数バッファ
	Comptr<ID3D12Resource> constBuff_;
	//ライト光線方向(単位ベクトル)
	XMVECTOR lightDir_ = { 1,0,0,0};
	//ライト色
	Vector3 lightColor_ = { 1,1,1 };
	//有効フラグ
	bool active_ = false;
	//ダーティフラグ(値に変更があったときだけ定数バッファに転送)
	bool dirty_ = false;
};
