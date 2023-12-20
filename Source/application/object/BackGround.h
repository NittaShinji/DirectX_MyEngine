#pragma once
#include"Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "LevelManager.h"

class BackGround : public Object3d
{
public:

	BackGround();
	~BackGround();

	void Initialize();

	void Update(Camera* camera);

	void Draw();

private:

	//ステージ情報
	LevelData* levelData_ = nullptr;

	std::string name_;
	std::unordered_map<std::string, Model> models_;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<Object3d>> objects_;
};

