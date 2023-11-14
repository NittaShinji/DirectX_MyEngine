#include "Stage.h"
#include "HitWall.h"
#include "CollisionAttribute.h"
#include <string>

void Stage::Initialize(const std::string& fileName)
{
	goalPos_ = { 0,0,0 };

	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(fileName);

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成
		if(objectData.fileName == "sphere" || objectData.fileName == "skydome" || objectData.fileName == "Plane" || objectData.fileName == "testStage0" || objectData.fileName == "Cube")
		{
			//3Dオブジェクトの生成
			std::unique_ptr<TouchableObject> newObject = nullptr;
			if(objectData.attribute == "Pink")
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_PINK);
			}
			else if(objectData.attribute == "Yellow")
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_YELLOW);
			}
			else if(objectData.attribute == "Goal")
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_GOAL);
			}
			else
			{
				newObject = TouchableObject::Create(objectData.fileName, COLLISION_ATTR_BLACK);
			}


			
			//座標
			Vector3 pos;
			pos = objectData.translation;
			newObject->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newObject->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newObject->SetScale(scale);

			if(objectData.attribute == "Pink")
			{
				newObject->SetAttributeColor(Attribute::pink);
			}
			else if(objectData.attribute == "Yellow")
			{
				newObject->SetAttributeColor(Attribute::yellow);
			}
			else if(objectData.attribute == "Goal")
			{
				goalPos_ = pos;
				newObject->SetAttributeColor(Attribute::Goal);
			}
			else
			{
				newObject->SetAttributeColor(Attribute::black);
			}

			if(objectData.fileName == "sphere" || objectData.fileName == "testStage0" || objectData.fileName == "Cube")
			{
				if(newObject->GetColorFlag() == false)
				{
					newObject->SetColorFlag(true);

					if(newObject->GetAttributeColor() == Attribute::yellow)
					{
						newObject->SetColor(Vector3(1.0f, 0.469f, 0.0f));
					}
					else if(newObject->GetAttributeColor() == Attribute::pink)
					{
						newObject->SetColor(Vector3(0.78f, 0.08f, 0.52f));
					}
					else
					{
						newObject->SetColor(Vector3(0.78f, 0.78f, 0.78f));
					}
				}
				else if(newObject->GetColorFlag() == true)
				{
					newObject->SetColorFlag(false);
				}
			}

			//配列に登録
			objects_.push_back(std::move(newObject));
		}

		if(objectData.fileName == "wall")
		{
			//衝突壁オブジェクトの生成
			std::unique_ptr<HitWall> newWall = nullptr;
			newWall = HitWall::Create(objectData.fileName);

			//座標
			Vector3 pos;
			pos = objectData.translation;
			newWall->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newWall->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newWall->SetScale(scale);

			//その他の初期化
			newWall->Initialize();

			//配列に登録
			walls_.push_back(std::move(newWall));
		}
	}
}

void Stage::Update(Camera* camera, bool isPlayerAccelerating)
{
	for(auto& object : objects_)
	{
		object->Update(camera);
	}

	for(size_t i = 0; i < walls_.size(); i++)
	{
		walls_[i]->Update(camera, isPlayerAccelerating);

		//壁が壊れていたら削除
		if(walls_[i]->GetIsBreak_())
		{
			walls_.erase(walls_.begin() + i);
			i = (size_t)-1;
		}
	}
}

void Stage::Draw()
{
	for(auto& object : objects_)
	{
		object->Draw();
	}

	for(auto& wall : walls_)
	{
		wall->Draw();
	}
}

void Stage::Reset(const std::string& fileName)
{
	//壁を削除
	walls_.clear();

	//壁を再配置
	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(fileName);

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }

		if(objectData.fileName == "wall")
		{
			//衝突壁オブジェクトの生成
			std::unique_ptr<HitWall> newWall = nullptr;
			newWall = HitWall::Create(objectData.fileName);

			//座標
			Vector3 pos;
			pos = objectData.translation;
			newWall->SetTransform(pos);

			//回転角
			Vector3 rot;
			rot = objectData.rotation;
			newWall->SetRotation(rot);

			//大きさ
			Vector3 scale;
			scale = objectData.scaling;
			newWall->SetScale(scale);

			//その他の初期化
			newWall->Initialize();
			
			//配列に登録
			walls_.push_back(std::move(newWall));
		}
	}
}

Vector3 Stage::GetGoalPos()
{
	return goalPos_;
}
