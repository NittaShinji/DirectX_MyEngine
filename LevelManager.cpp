#include "LevelManager.h"
#include "json.hpp"
#include <string>
#include <cassert>

std::string LevelManager::kDefaultJSONDirectoryPath_ = "Resources/";

void LevelManager::Return(nlohmann::json deserialized)
{
	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for(nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理

		//タイプが"MESH"だった場合
		if(type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::objectDate{});
			//今追加した要素の参照を得る
			LevelData::objectDate& objectData = levelData->objects.back();

			if(object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			//objectData.translation.r->m128_f32[0] = (float)transform["translation"][1];
			//objectData.translation.r->m128_f32[1] = (float)transform["translation"][2];
			//objectData.translation.r->m128_f32[2] = -(float)transform["translation"][0];
			//objectData.translation.r->m128_f32[3] = 1.0f;
			////回転角
			//objectData.rotation.r->m128_f32[0] = -(float)transform["rotation"][1];
			//objectData.rotation.r->m128_f32[1] = -(float)transform["rotation"][2];
			//objectData.rotation.r->m128_f32[2] = (float)transform["rotation"][0];
			//objectData.rotation.r->m128_f32[3] = 0.0f;
			////スケーリング
			//objectData.scaling.r->m128_f32[0] = (float)transform["scaling"][1];
			//objectData.scaling.r->m128_f32[1] = (float)transform["scaling"][2];
			//objectData.scaling.r->m128_f32[2] = (float)transform["scaling"][0];
			//objectData.scaling.r->m128_f32[3] = 0.0f;
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];

			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][0];

			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];
		}
	}
}

LevelData* LevelManager::LoadJSONFile(const std::string& fileName)
{
	//連結してフルパスを得る
	const std::string fullPath = kDefaultJSONDirectoryPath_ + fileName;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullPath);
	//ファイルオープン失敗をチェック
	if(file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for(nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理

		//タイプが"MESH"だった場合
		if(type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::objectDate{});
			//今追加した要素の参照を得る
			LevelData::objectDate& objectData = levelData->objects.back();

			if(object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y= (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];
			
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][0];
			
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];
			
		}

		//再帰処理
		if(object.contains("children")) 
		{
			//Return(deserialized);
		}
		/*else
		{
			levelData_ = *levelData;
			break;
		}*/
	}

	return levelData;
}
