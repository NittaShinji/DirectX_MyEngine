#include "Stage.h"
#include <string>
#include <DirectXMath.h>

void Stage::Initialize()
{
	//レベルデータからオブジェクトを生成、配置
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile("Stage0.json");

	for(auto& objectData : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//モデルを指定して3Dオブジェクトを作成

		if(objectData.fileName == "sphere" || objectData.fileName == "testStage0")
		{
			//モデルをロード
			Model::Load(objectData.fileName);

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

			attribute_ = objectData.attribute;

			//配列に登録
			objects_.push_back(std::move(newObject));
		}
	}

	/*colorFlag_ = false;*/
}

void Stage::Update(Camera* camera)
{
	/*std::vector<std::unique_ptr<TouchableObject>>::iterator itr = objects_.begin();

	for(; itr < objects_.end(); itr++)
	{
		if(KeyInput::GetInstance()->PushedKeyMoment(DIK_7))
		{
			if((*itr)->GetColorFlag() == false)
			{
				(*itr)->SetColorFlag(true);
				(*itr)->SetColor(XMFLOAT3(0.0f, 0.0f, 1.0f));
			}
			else if((*itr)->GetColorFlag() == true)
			{
				(*itr)->SetColorFlag(false);
			}
		}

		(*itr)->Update(camera);
	}*/

	for(auto& object : objects_)
	{
		if(KeyInput::GetInstance()->PushedKeyMoment(DIK_7))
		{
			//if(colorFlag_ == false)
			//{
			//	colorFlag_ = true;

			//	object->SetColor(XMFLOAT3(0.0f, 0.0f, 1.0f));
			//}

			//if(colorFlag_ == true)
			//{
			//	colorFlag_ = false;
			//}

			if(object->GetColorFlag() == false)
			{
				object->SetColorFlag(true);

				object->SetColor(XMFLOAT3(0.0f, 0.0f, 1.0f));
			}

			else if(object->GetColorFlag() == true)
			{
				object->SetColorFlag(false);
			}
		}
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
