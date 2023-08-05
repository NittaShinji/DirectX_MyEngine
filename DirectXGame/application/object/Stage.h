#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class Camera;
class Player;

class Stage : public TouchableObject
{
private:

	//nameSpace
	//using XMFLOAT = DirectX::Vector3;
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	//using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize();

	void Update(Camera* camera);

	void Draw();

	Vector3 GetGoalPos();

private:

	//ステージ情報
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;
	MODELKEY name_;
	std::unordered_map<MODELKEY, Model> models_;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<TouchableObject>> objects_;

	//ゴール座標
	Vector3 goalPos_;
};

