#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	scene_->Finalize();
}

void SceneManager::Initialize()
{


}

void SceneManager::Update()
{
	//�V�[���ؑ֋@�\
	if(nextScene_)
	{
		//���V�[���̏I��
		if(scene_)
		{
			scene_->Finalize();
		}

		//�V�[���ؑ�
		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;

		//�V�[���}�l�[�W�����Z�b�g
		scene_->SetSceneManager(this);

		//���V�[��������������
		scene_->Initialize();
	}

	//���s���V�[�����X�V����
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//���V�[���𐶐�
	nextScene_ = std::move(sceneFactory_->CreateScene(sceneName));
}
