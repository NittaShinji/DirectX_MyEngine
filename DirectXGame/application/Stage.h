#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"

class Camera;

class Stage : public TouchableObject
{
private:

	//nameSpace
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	enum Attribute
	{
		pink = 0,
		yellow,
		black,
	};

	void Initialize();

	void Update(Camera* camera);

	void Draw();

private:

	//ステージ情報
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;
	MODELKEY name_;
	std::unordered_map<MODELKEY, Model> models_;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<TouchableObject>> objects_;

	//std::vector<bool> colorFlag_;
};

