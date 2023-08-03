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
	//�I�u�W�F�N�g
	struct objectDate
	{
		/*XMMATRIX translation;
		XMMATRIX rotation;
		XMMATRIX scaling;*/

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

private: //�G�C���A�X

	//Microsoft::WRL::���ȗ�
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	//using Vector2 = DirectX::Vector2;
	//using Vector3 = DirectX::Vector3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//JSON�t�@�C���ǂݍ���
	LevelData* LoadJSONFile(const std::string& fileName);
	//�ċA�֐�
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

