#pragma once
#include "ImGuiManager.h"
#include "GameScene.h"
#include "PostEffect.h"
#include "MyFramework.h"
#include "TitleScene.h"
#include <fstream>
#include <memory>

class MyGame : public MyFramework
{

public: //�����o�֐�

	MyGame();

	//������
	void Initialize() override;
	//�I��
	void Finalize() override;
	//���t���[���X�V
	void Update() override;
	//�`��
	void Draw() override;

private: //�����o�ϐ�

	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_ = nullptr;
};

