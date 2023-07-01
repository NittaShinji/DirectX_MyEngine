#include "LevelManager.h"
#include "MathUtillity.h"
#include "json.hpp"
#include <string>
#include <cassert>

void LevelManager::Return(nlohmann::json& object, LevelData* levelData)
{
	assert(object.contains("type"));

	//��ʂ��擾
	std::string type = object["type"].get<std::string>();
	//bool visible = object["visible"].get<bool>();

	//��ނ��Ƃ̏���

	//�^�C�v��"MESH"�������ꍇ
	if(type.compare("MESH") == 0)
	{

		//�v�f�ǉ�
		levelData->objects.emplace_back(LevelData::objectDate{});
		//���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::objectDate& objectData = levelData->objects.back();

		if(object.contains("file_name"))
		{
			//�t�@�C����
			objectData.fileName = object["file_name"];
		}
		//else if(visible == true)
		//{
		//	//�t�@�C����
		//	objectData.fileName = object["visible"];
		//}


		//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
		nlohmann::json& transform = object["transform"];
		////���s�ړ�
		objectData.translation.x = (float)transform["translation"][1];
		objectData.translation.y = (float)transform["translation"][2];
		objectData.translation.z = -(float)transform["translation"][0];

		//��]�p
		objectData.rotation.x = -(float)transform["rotation"][1];
		objectData.rotation.y = -(float)transform["rotation"][2];
		objectData.rotation.z = (float)transform["rotation"][0];

		objectData.rotation.x = MathUtillty::ToRadian(objectData.rotation.x);
		objectData.rotation.y = MathUtillty::ToRadian(objectData.rotation.y);
		objectData.rotation.z = MathUtillty::ToRadian(objectData.rotation.z);

		//�X�P�[�����O
		objectData.scaling.x = (float)transform["scaling"][1];
		objectData.scaling.y = (float)transform["scaling"][2];
		objectData.scaling.z = (float)transform["scaling"][0];

		//���s�ړ�
		//objectData.translation.r->m128_f32[0] = (float)transform["translation"][1];
		//objectData.translation.r->m128_f32[1] = (float)transform["translation"][2];
		//objectData.translation.r->m128_f32[2] = -(float)transform["translation"][0];
		//objectData.translation.r->m128_f32[3] = 1.0f;

		////��]�p
		//objectData.rotation.r->m128_f32[0] = -(float)transform["rotation"][1];
		//objectData.rotation.r->m128_f32[1] = -(float)transform["rotation"][2];
		//objectData.rotation.r->m128_f32[2] = (float)transform["rotation"][0];
		//objectData.rotation.r->m128_f32[3] = 0.0f;


		////�X�P�[�����O
		//objectData.scaling.r->m128_f32[0] = (float)transform["scaling"][1];
		//objectData.scaling.r->m128_f32[1] = (float)transform["scaling"][2];
		//objectData.scaling.r->m128_f32[2] = (float)transform["scaling"][0];
		//objectData.scaling.r->m128_f32[3] = 0.0f;

	}



	//�ċA����
	if(object.contains("children"))
	{
		for(nlohmann::json& object_ : object["children"])
		{
			Return(object_, levelData);
		}
	}
}

void LevelManager::LoadElement(nlohmann::json& object, LevelData* levelData)
{
	////�v�f�ǉ�
	//levelData->objects.emplace_back(LevelData::objectDate{});
	////���ǉ������v�f�̎Q�Ƃ𓾂�
	//LevelData::objectDate& objectData = levelData->objects.back();

	//if(object.contains("file_name"))
	//{
	//	//�t�@�C����
	//	objectData.fileName = object["file_name"];
	//}

	////�g�����X�t�H�[���̃p�����[�^�ǂݍ���
	//nlohmann::json& transform = object["transform"];
	////���s�ړ�
	//objectData.translation.x = (float)transform["translation"][1];
	//objectData.translation.y = (float)transform["translation"][2];
	//objectData.translation.z = (float)transform["translation"][0];

	////��]�p
	//objectData.rotation.x = -(float)transform["rotation"][1];
	//objectData.rotation.y = -(float)transform["rotation"][2];
	//objectData.rotation.z = -(float)transform["rotation"][0];

	////�X�P�[�����O
	//objectData.scaling.x = (float)transform["scaling"][1];
	//objectData.scaling.y = (float)transform["scaling"][2];
	//objectData.scaling.z = (float)transform["scaling"][0];

}

LevelData* LevelManager::LoadJSONFile(const std::string& fileName)
{
	//�f�t�H���gJSON�i�[�f�B���N�g��
	std::string kDefaultJSONDirectoryPath_ = "Resources/";

	//�A�����ăt���p�X�𓾂�
	const std::string fullPath = kDefaultJSONDirectoryPath_ + fileName;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullPath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if(file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for(nlohmann::json& object : deserialized["objects"])
	{
		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//�^�C�v��"LIGHT"�������ꍇ
		if(type.compare("LIGHT") == 0)
		{
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::objectDate{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::objectDate& objectData = levelData->objects.back();

			//�t�@�C����
			objectData.fileName = "LIGHT";

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];

			//��]�p
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][0];

			//�X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//���s�ړ�
			//objectData.translation.r->m128_f32[0] = (float)transform["translation"][1];
			//objectData.translation.r->m128_f32[1] = (float)transform["translation"][2];
			//objectData.translation.r->m128_f32[2] = -(float)transform["translation"][0];
			//objectData.translation.r->m128_f32[3] = 1.0f;

			////��]�p
			//objectData.rotation.r->m128_f32[0] = -(float)transform["rotation"][1];
			//objectData.rotation.r->m128_f32[1] = -(float)transform["rotation"][2];
			//objectData.rotation.r->m128_f32[2] = (float)transform["rotation"][0];
			//objectData.rotation.r->m128_f32[3] = 0.0f;


			////�X�P�[�����O
			//objectData.scaling.r->m128_f32[0] = (float)transform["scaling"][1];
			//objectData.scaling.r->m128_f32[1] = (float)transform["scaling"][2];
			//objectData.scaling.r->m128_f32[2] = (float)transform["scaling"][0];
			//objectData.scaling.r->m128_f32[3] = 0.0f;

			//LoadElement(object, levelData);
		}
		if(type.compare("CAMERA") == 0)
		{
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::objectDate{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::objectDate& objectData = levelData->objects.back();

			//�t�@�C����
			objectData.fileName = "CAMERA";

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];

			//���s�ړ�
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][0];

			//��]�p
			objectData.rotation.x = -(float)transform["rotation"][1];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][0];

			//�X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][1];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][0];

			//���s�ړ�
			//objectData.translation.r->m128_f32[0] = (float)transform["translation"][1];
			//objectData.translation.r->m128_f32[1] = (float)transform["translation"][2];
			//objectData.translation.r->m128_f32[2] = -(float)transform["translation"][0];
			//objectData.translation.r->m128_f32[3] = 1.0f;

			////��]�p
			//objectData.rotation.r->m128_f32[0] = -(float)transform["rotation"][1];
			//objectData.rotation.r->m128_f32[1] = -(float)transform["rotation"][2];
			//objectData.rotation.r->m128_f32[2] = (float)transform["rotation"][0];
			//objectData.rotation.r->m128_f32[3] = 0.0f;


			////�X�P�[�����O
			//objectData.scaling.r->m128_f32[0] = (float)transform["scaling"][1];
			//objectData.scaling.r->m128_f32[1] = (float)transform["scaling"][2];
			//objectData.scaling.r->m128_f32[2] = (float)transform["scaling"][0];
			//objectData.scaling.r->m128_f32[3] = 0.0f;
			//LoadElement(object, levelData);
		}

		Return(object, levelData);
	}

	return levelData;
}
