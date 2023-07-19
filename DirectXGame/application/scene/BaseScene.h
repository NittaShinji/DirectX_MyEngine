#pragma once
#include "DirectXBasic.h"
#include "ImGuiManager.h"
#include "Input.h"

//�O���錾
class SceneManager;

//�V�[�����N���X
class BaseScene
{
public:

	//static void StaticInitialize(DirectXBasic* directXBasic);
	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	//������
	virtual void Initialize();

	//���t���[���X�V
	virtual void Update();

	//�I��
	virtual void Finalize();

	//�`��
	virtual void Draw();

public: //�����o�֐�

	virtual ~BaseScene() = default;
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:

	static DirectXBasic* directXBasic_;

	static ImGuiManager* imGuiManager_;

	//�V�[���}�l�[�W��
	SceneManager* sceneManager_ = nullptr;
};

