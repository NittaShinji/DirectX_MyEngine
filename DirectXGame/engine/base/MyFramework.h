#pragma once
#include "WindowsAPI.h"
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "GameScene.h"
#include "Input.h"

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

	//�Q�[���I���t���O
	bool endRequst_ = false;

};

