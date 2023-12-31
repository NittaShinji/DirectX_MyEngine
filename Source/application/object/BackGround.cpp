#include "BackGround.h"
#include "ObjectAttribute.h"
#include "ObjectColor.h"

BackGround::BackGround(){}

BackGround::~BackGround(){}

void BackGround::Initialize(const std::string fileName)
{
	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile(fileName);

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		std::unordered_map<std::string, Model> models_;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成

		if(objectData.fileName == "Cube" || objectData.fileName == "wall"  || objectData.fileName == "StageBlock")
		{
			//3Dオブジェクトの生成
			std::unique_ptr<Object3d> newObject = nullptr;
			newObject = Object3d::Create(objectData.fileName);

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

			//属性指定
			if(objectData.attribute == "Pink")
			{
				newObject->SetAttributeColor(Attribute::pink);
			}
			else if(objectData.attribute == "Yellow")
			{
				newObject->SetAttributeColor(Attribute::yellow);
			}
			else if(objectData.attribute == "Black")
			{
				newObject->SetAttributeColor(Attribute::black);
			}
			else if(objectData.attribute == "NreverseOBJ")
			{
				newObject->SetAttributeColor(Attribute::NReverseObj);
			}
			else
			{
				newObject->SetAttributeColor(Attribute::NormalObj);
			}

			//色指定
			if(newObject->GetColorFlag() == false)
			{
				newObject->SetColorFlag(true);

				if(newObject->GetAttributeColor() == Attribute::yellow)
				{
					newObject->SetColor(kYellowOBJColor);
				}
				else if(newObject->GetAttributeColor() == Attribute::pink)
				{
					newObject->SetColor(kPinkOBJColor);
				}
				else if(newObject->GetAttributeColor() == Attribute::black)
				{
					newObject->SetColor(kBlackOBJColor);
				}
				else
				{
					if(newObject->GetAttributeColor() == Attribute::NReverseObj)
					{
						newObject->SetColor(kNormalReverseOBJColor);
					}

					if(newObject->GetAttributeColor() == Attribute::NormalObj)
					{
						newObject->SetColor(kNormalOBJColor);
					}
				}
			}
			
			//配列に登録
			objects_.push_back(std::move(newObject));
		}
	}
}

void BackGround::Update(Camera* camera)
{
	for(auto& object : objects_)
	{
		object->Update(camera);
	}
}

void BackGround::Draw()
{
	for(auto& object : objects_)
	{
		object->Draw();
	}
}
