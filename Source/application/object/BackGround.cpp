#include "BackGround.h"
#include "ObjectAttribute.h"

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
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成

		if(objectData.fileName == "Cube" || objectData.fileName == "wall"  || objectData.fileName == "testStage0")
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
				newObject->SetAlpha(1.0f);

				if(newObject->GetAttributeColor() == Attribute::yellow)
				{
					newObject->SetColor(Vector3(1.0f, 0.469f, 0.0f));
				}
				else if(newObject->GetAttributeColor() == Attribute::pink)
				{
					newObject->SetColor(Vector3(0.78f, 0.08f, 0.52f));
				}
				else if(newObject->GetAttributeColor() == Attribute::black)
				{
					newObject->SetColor(Vector3(0.0f, 0.0f, 0.0f));
				}
				else
				{
					if(newObject->GetAttributeColor() == Attribute::NReverseObj)
					{
						newObject->SetColor(Vector3(0.6f, 0.6f, 0.65f));
						newObject->SetAlpha(0.7f);
					}

					if(newObject->GetAttributeColor() == Attribute::NormalObj)
					{
						newObject->SetColor(Vector3(0.7f, 0.7f, 0.7f));
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
