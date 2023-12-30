#pragma once
#include "Object3d.h"

class MeshCollider;

/// <summary>
/// 接触可能オブジェクト
/// 自動で当たり判定を登録するオブジェクトクラス
/// </summary>
class TouchableObject : public Object3d
{
public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	static std::unique_ptr<TouchableObject> Create(const std::string& path,const unsigned short coliderAttribute);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	void Initialize();

	//コライダーの追加
	void AddCollider(Model* model);

	//コライダーの更新
	void ColliderUpdate();

	//描画
	void Draw() override;

private:

	//メッシュコライダー
	std::unique_ptr<MeshCollider> objMeshCollider_;
};

