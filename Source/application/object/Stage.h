#pragma once
#include "TouchableObject.h"
#include "HitWall.h"
#include "GoalOBJ.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class Camera;
class Player;

class Stage : public TouchableObject
{
public:

	void Initialize(const std::string& fileName);

	void Update(Camera* camera, Player* player);

	void Draw();

	void Reset(const std::string& fileName);

	Vector3 GetGoalPos();

	std::vector<Vector3> GetBreakWallsPos();

	GoalOBJ* GetGoal() { return goal_.get(); }

private:

	//ステージ情報
	LevelData* levelData_ = nullptr;

	std::string name_;
	std::unordered_map<std::string, Model> models_;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<TouchableObject>> objects_;
	std::vector<std::unique_ptr<HitWall>> walls_;
	std::unique_ptr<GoalOBJ> goal_;

	//ゴール座標
	Vector3 goalPos_;
};

