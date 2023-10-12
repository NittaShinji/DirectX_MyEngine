#pragma once
#include "TouchableObject.h"
#include "HitWall.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class Camera;
class Player;

class Stage : public TouchableObject
{
public:

	void Initialize(const std::string& fileName);

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
	std::vector<std::unique_ptr<HitWall>> walls_;


	//ゴール座標
	Vector3 goalPos_;
};

