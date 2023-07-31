#include "Stage.h"
#include <string>
#include <DirectXMath.h>

void Stage::Initialize()
{
	goalPos_ = { 0,0,0 };

	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile("Stage0.json");

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成

		if(objectData.fileName == "sphere" || objectData.fileName == "testStage0" || objectData.fileName == "skydome")
		{
			//3Dオブジェクトの生成
			std::unique_ptr<TouchableObject> newObject = nullptr;
			newObject = TouchableObject::Create(objectData.fileName);

			DirectX::XMFLOAT3 pos;
			pos = objectData.translation;
			newObject->SetTransform(pos);

			//回転角
			DirectX::XMFLOAT3 rot;
			rot = objectData.rotation;
			newObject->SetRotation(rot);

			//大きさ
			DirectX::XMFLOAT3 scale;
			scale = objectData.scaling;
			newObject->SetScale(scale);

			if(objectData.attribute == "Pink")
			{
				newObject->SetAttribute(Attribute::pink);
			}
			else if(objectData.attribute == "Yellow")
			{
				newObject->SetAttribute(Attribute::yellow);
			}
			else if(objectData.attribute == "Goal")
			{
				goalPos_ = pos;
				newObject->SetAttribute(Attribute::Goal);
			}
			else
			{
				newObject->SetAttribute(Attribute::black);
			}

			if(objectData.fileName == "sphere" || objectData.fileName == "testStage0")
			{
				if(newObject->GetColorFlag() == false)
				{
					newObject->SetColorFlag(true);

					if(newObject->GetAttribute() == Attribute::yellow)
					{
						newObject->SetColor(XMFLOAT3(1.0f, 0.469f, 0.0f));
					}
					else if(newObject->GetAttribute() == Attribute::pink)
					{
						newObject->SetColor(XMFLOAT3(0.78f, 0.08f, 0.52f));
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
	}
}

void Stage::Update(Camera* camera)
{
	for(auto& object : objects_)
	{
		object->Update(camera);
	}
}

void Stage::Draw()
{
	for(auto& object : objects_)
	{
		object->Draw();
	}
}

XMFLOAT3 Stage::GetGoalPos()
{
	return goalPos_;
}
