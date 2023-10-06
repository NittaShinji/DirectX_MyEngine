#pragma once
#include "json.hpp"
#include "Object3d.h"
#include "Model.h"
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include "Vector3.h"

struct LevelData
{
	//オブジェクト
	struct objectDate
	{
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		std::string fileName;
		std::string attribute;
	};

	std::vector<objectDate> objects;
};

class LevelManager
{

private: //エイリアス

	//Microsoft::WRL::を省略
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;

public:

	//JSONファイル読み込み
	LevelData* LoadJSONFile(const std::string& fileName);
	//再帰関数
	void Return(nlohmann::json& deserialized, LevelData* levelData);

	LevelData GetLevelData() { return levelData_; };

	static LevelManager* GetLevelManager()
	{
		static LevelManager levelManager;
		return &levelManager;
	};


private:

	LevelManager() {};
	~LevelManager() {};

	LevelData levelData_;
};

