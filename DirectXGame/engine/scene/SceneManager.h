#pragma once
#include "BaseScene.h"

class SceneManager
{

public: //�����o�֐�

	SceneManager();

	~SceneManager();

	void Initialize();

	void Update();

	void Draw();

	//���̃V�[����\��
	void SetNextScene(std::unique_ptr<BaseScene> nextScene) { nextScene_ = std::move(nextScene); }

private:

	//���̃V�[��
	//BaseScene* nextScene_ = nullptr;
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	//���̃V�[��(���s���V�[��)
	//BaseScene* scene_ = nullptr;
	std::unique_ptr<BaseScene> scene_ = nullptr;

};

