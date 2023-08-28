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
	std::unique_ptr<PostEffect> postEffect2_ = nullptr;


	//���C�g
	LightGroup* lightGroup_ = nullptr;

	//�w�i
	std::unique_ptr<BackGround> backGround_ = nullptr;
	//�w�i�I�u�W�F�N�g�p�̃J����
	std::unique_ptr<Camera> gameCamera_ = nullptr;

};

