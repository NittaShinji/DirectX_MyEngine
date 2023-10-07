#pragma once
#include "Object3d.h"
#include "Model.h"
#include <fstream>
#include <vector>
#include "Vector3.h"

#pragma warning(push)
#pragma warning(disable:4623)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4820)
#pragma warning(disable:5027)

#include "json.hpp"
#pragma warning(pop)

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

