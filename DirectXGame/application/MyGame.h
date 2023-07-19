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

	//ImGui����������
	//std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;
	////��ՃV�[��
	//std::unique_ptr<BaseScene> scene_ = nullptr;
	////�^�C�g���V�[��
	//std::unique_ptr<TitleScene> titleScene_ = nullptr;
	////�Q�[���V�[��
	//std::unique_ptr<GameScene> gameScene_ = nullptr;
	//
	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_ = nullptr;
};

