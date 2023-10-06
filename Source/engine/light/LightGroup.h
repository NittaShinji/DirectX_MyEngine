
#pragma once
#include "DirectionalLight.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "Vector2.h"
#include "Vector3.h"

///<summary>
/// ライト
///<summary>
class LightGroup
{

private: //エイリアス

	//Microsoft::WRL::を省略
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;

public: //静的メンバ変数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

public: //メンバ関数


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <retums>インスタンス</retums>
	static LightGroup* Create();

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 環境光のライト色をセット
	/// </summary>
	/// <param name="color">ライト色</param>
	void SetAmbientColor(const Vector3& color);

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	///  <param name="index">ライト番号</param>
	///  <param name="active">有効フラグ</param>
	void SetDirLightActive(int32_t index, bool active);

	/// <summary>
	/// 平行光源のライトをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightDir">ライト方向</param>
	void SetDirLightDir(int32_t index, const Vector3& lightDir, const float upVec);


	/// <summary>
	/// 平行光源のライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightColor">ライト色</param>
	void SetDirLightColor(int32_t index, const Vector3& lightColor);


	/// <summary>
	/// 標準のライトの設定
	/// </summary>
	void DefaultLightSetting();

public: //定数
	//ライトの数
	static const int32_t kDirLightNum_ = 3;
	//点光源の数
	static const int32_t kPointLightNum_ = 3;
	//スポットライトの数
	static const int32_t kSpotLightNum_ = 3;
	//丸影の数
	static const int32_t kCircleShadowNum_ = 1;


public: //サブクラス

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		//環境光の色
		Vector3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[kDirLightNum_];
	};


private: //静的メンバ変数

	//デバイス
	static ID3D12Device* device_;

private: //メンバ変数

	//定数バッファ
	Comptr<ID3D12Resource> constBuff_;
	//環境光の色
	Vector3 ambientColor_ = { 1,1,1 };
	//平行光源の配列
	DirectionalLight dirLights_[kDirLightNum_];
	//ダーティフラグ
	bool dirty_ = false;

};

