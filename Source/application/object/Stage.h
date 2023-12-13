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

	void Update(Camera* camera, bool isPlayerAxcell);

	void Draw();

	void Reset(const std::string& fileName);

	Vector3 GetGoalPos();

	std::vector<Vector3> GetBreakWallsPos();

private:

	//ステージ情報
	LevelData* levelData_ = nullptr;

	//using MODELKEY = std::string;
	std::string name_;
	std::unordered_map<std::string, Model> models_;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<TouchableObject>> objects_;
	std::vector<std::unique_ptr<HitWall>> walls_;

	//ゴール座標
	Vector3 goalPos_;
};

