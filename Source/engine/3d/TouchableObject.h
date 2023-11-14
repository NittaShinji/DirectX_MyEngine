#pragma once
#include "Object3d.h"

class MeshCollider;

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

	void ColliderUpdate();

	void Draw() override;

private:

	std::unique_ptr<MeshCollider> objMeshCollider_;
};

