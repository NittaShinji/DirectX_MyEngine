#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Sound.h"
#include "Input.h"
#include "SceneManager.h"

//�Q�[���S��
class MyFramework
{
public:	//�����o�֐�

	//������
	virtual void Initialize();

	//���t���[���X�V
	virtual void Update();

	//�I��
	virtual void Finalize();

	//�`��
	virtual void Draw() = 0;

	//�I���`�F�b�N
	virtual bool IsEndRequst() { return endRequst_; }

	//���s
	void Run();

public: //�����o�֐�

	//���z�f�X�g���N�^
	virtual ~MyFramework() = default;

protected: //�����o�ϐ�

	//WindowsApi
	std::unique_ptr<WindowsAPI> winApi_ = nullptr;

	//DirectX���
	std::unique_ptr<DirectXBasic> directXBasic_ = nullptr;

	//���͏���
	std::unique_ptr<Input> input_ = nullptr;

	//ImGui����������
	std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;

	//�T�E���h
	//std::unique_ptr<Sound> sound_ = nullptr;

	//�X�v���C�g
	SpriteCommon* spriteCommon_ = nullptr;

	//��ՃV�[��
	std::unique_ptr<SceneManager> sceneManager_ = nullptr;
	//std::unique_ptr<BaseScene> scene_ = nullptr;

	//�Q�[���I���t���O
	bool endRequst_ = false;

};

