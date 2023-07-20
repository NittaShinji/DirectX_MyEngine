#pragma once
#include "BaseScene.h"

class SceneManager final
{

public: //�����o�֐�

	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}

	void Initialize();

	void Update();

	void Draw();

	//���̃V�[����\��
	void SetNextScene(std::unique_ptr<BaseScene> nextScene) { nextScene_ = std::move(nextScene); }

private:

	SceneManager();
	~SceneManager();
	//�R�s�[�R���X�g���N�^�̖���
	SceneManager(const SceneManager& sceneManager) = delete;
	//������Z�q�̖���
	SceneManager& operator=(const SceneManager& sceneManager) = delete;

	//���̃V�[��
	std::unique_ptr<BaseScene> nextScene_ = nullptr;
	//���̃V�[��(���s���V�[��)
	std::unique_ptr<BaseScene> scene_ = nullptr;

};

