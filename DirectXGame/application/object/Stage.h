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
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize();

	void Update(Camera* camera);

	void Draw();

	XMFLOAT3 GetGoalPos();

private:

	//�X�e�[�W���
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;
	MODELKEY name_;
	std::unordered_map<MODELKEY, Model> models_;

	//blender�ǂݍ��݃I�u�W�F�N�g
	std::vector<std::unique_ptr<TouchableObject>> objects_;

	//�S�[�����W
	XMFLOAT3 goalPos_;
};
