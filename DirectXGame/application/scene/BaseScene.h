#pragma once
#include "DirectXBasic.h"
#include "ImGuiManager.h"

//�V�[�����N���X
class BaseScene
{
public:

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

protected:

	static DirectXBasic* directXBasic_;
	static ImGuiManager* imGuiManager_;
};

