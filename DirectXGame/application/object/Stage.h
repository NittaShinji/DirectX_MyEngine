#pragma once
#include "TouchableObject.h"
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

	//�X�e�[�W���
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;
	MODELKEY name_;
	std::unordered_map<MODELKEY, Model> models_;

	//blender�ǂݍ��݃I�u�W�F�N�g
	std::vector<std::unique_ptr<TouchableObject>> objects_;

	//�S�[�����W
	Vector3 goalPos_;
};

