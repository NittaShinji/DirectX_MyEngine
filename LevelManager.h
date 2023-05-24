#pragma once
#include "json.hpp"
#include "Object3d.h"
#include "Model.h"
#include <DirectXMath.h>
#include <fstream>
#include <vector>

struct LevelData
{
	//�I�u�W�F�N�g
	struct objectDate
	{
		XMFLOAT3 translation;
		XMFLOAT3 rotation;
		XMFLOAT3 scaling;
		std::string fileName;
	};

	std::vector<objectDate> objects;
};

class LevelManager
{

private: //�G�C���A�X

	//Microsoft::WRL::���ȗ�
	template <class T> using Comptr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//JSON�t�@�C���ǂݍ���
	LevelData* LoadJSONFile(const std::string& fileName);
	//�ċA�֐�
	void Return(nlohmann::json deserialized, LevelData* levelData);
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

	//std::unique_ptr<Object3d> object3d_;

};

