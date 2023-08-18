#include "Stage.h"
#include <string>
#include <DirectXMath.h>

void Stage::Initialize()
{
	goalPos_ = { 0,0,0 };

	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile("Stage0.json");

	for(auto& objectData : levelData_->objects)
	{
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//���f�����w�肵��3D�I�u�W�F�N�g���쐬

		if(objectData.fileName == "sphere" || objectData.fileName == "testStage0" || objectData.fileName == "skydome")
		{
			//3D�I�u�W�F�N�g�̐���
			std::unique_ptr<TouchableObject> newObject = nullptr;
			newObject = TouchableObject::Create(objectData.fileName);

			Vector3 pos;

			pos = objectData.translation;
			newObject->SetTransform(pos);

			//��]�p
			Vector3 rot;
			rot = objectData.rotation;
			newObject->SetRotation(rot);

			//�傫��
			Vector3 scale;
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
						newObject->SetColor(Vector3(1.0f, 0.469f, 0.0f));
					}
					else if(newObject->GetAttribute() == Attribute::pink)
					{
						newObject->SetColor(Vector3(0.78f, 0.08f, 0.52f));
					}
				}
				else if(newObject->GetColorFlag() == true)
				{
					newObject->SetColorFlag(false);
				}
			}

			//�z��ɓo�^
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

Vector3 Stage::GetGoalPos()
{
	return goalPos_;
}
