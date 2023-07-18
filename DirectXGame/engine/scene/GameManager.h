#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "GameScene.h"
#include "PostEffect.h"
#include "xaudio2.h"
#include <fstream>
#include <memory>

class GameManager
{

public: //�����o�֐�

	GameManager();

	//������
	void Initialize();
	//�I��
	void Finalize();
	//���t���[���X�V
	void Update();
	//�`��
	void Draw();

	//�I���t���O�̃`�F�b�N
	bool IsEndRequst() { return endRequst_; }

private: //�����o�ϐ�

	std::unique_ptr<WindowsAPI> winApi_ = nullptr;

	//DirectX����������
	std::unique_ptr<DirectXBasic> directXBasic_ = nullptr;
	
	//ImGui����������
	std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;
	//���͏���
	std::unique_ptr<Input> input_ = nullptr;
	
	//�Q�[���V�[��
	std::unique_ptr<GameScene> gameScene_ = nullptr;
	
	//�X�v���C�g
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

	//�Q�[���I���t���O
	bool endRequst_ = false;
};

