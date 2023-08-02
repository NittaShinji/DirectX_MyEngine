#pragma once
#include "json.hpp"
#include "Object3d.h"
#include "Model.h"
#include <DirectXMath.h>
#include <fstream>
#include <vector>

struct LevelData
{
	//オブジェクト
	struct objectDate
	{
		/*XMMATRIX translation;
		XMMATRIX rotation;
		XMMATRIX scaling;*/

		XMFLOAT3 translation;
		XMFLOAT3 rotation;
		XMFLOAT3 scaling;
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
	//DirectX::を省略
	//using Vector2 = DirectX::Vector2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

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

