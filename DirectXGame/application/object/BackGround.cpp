#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

void BackGround::Initialize()
{
	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	levelData_ = LevelManager::GetLevelManager()->LoadJSONFile("backGround.json");

	for(auto& objectData : levelData_->objects)
	{
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if(it != models_.end()) { model = &it->second; }
		//���f�����w�肵��3D�I�u�W�F�N�g���쐬

		if(objectData.fileName == "Cube")
		{
			//3D�I�u�W�F�N�g�̐���
			std::unique_ptr<Object3d> newObject = nullptr;
			newObject = Object3d::Create(objectData.fileName);

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

			//�F�w��
			newObject->SetColorFlag(false);

			//�z��ɓo�^
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
