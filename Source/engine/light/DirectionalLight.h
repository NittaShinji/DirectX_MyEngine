#pragma once
#include <wrl.h>
#include "MathUtillity.h"
#pragma warning(push)
#pragma warning(disable:4820)
#include <d3d12.h>
#pragma warning(pop)

using namespace MathUtillty;

class DirectionalLight
{
private: //エイリアステンプレート

	//Microsoft::WRL::を省略
	template<class T>using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectXを省略

public: //サブクラス

	//定数バッファ用データ構造体
	struct  ConstBufferData
	{
		Vector3 lightv;		//ライトへ方向を表すベクトル
		float pad1;
		Vector3 lightColor;	//ライトの色
		bool active;	//有効フラグ
	};

public: //アクセッサ

	//ライト方向をセット
	void SetLightDir(const Vector3& lightDir, const float upVec)
	{
		//正規化してセット
		this->lightDir_ = Vector3Normalize(lightDir);
		this->upVec_ = upVec;
		dirty_ = true;
	}

	//ライト色のセット
	void SetLightColor(const Vector3& lightColor)
	{
		this->lightColor_ = lightColor;
		dirty_ = true;
	}

	Vector3 GetLightDir() { return lightDir_; };
	float GetUpVec() { return upVec_; }

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
	Vector3 lightDir_ = { 1,0,0 };
	float upVec_ = 0.0f;
	//ライト色
	Vector3 lightColor_ = { 1,1,1 };
	//有効フラグ
	bool active_ = false;
	//ダーティフラグ(値に変更があったときだけ定数バッファに転送)
	bool dirty_ = false;
};
