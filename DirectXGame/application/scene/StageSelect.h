#include "BaseScene.h"
#include "Sprite.h"
#include "GamePad.h"
#pragma once

class StageSelectScene : public BaseScene
{

public:

	//�ÓI������
	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	//������
	void Initialize() override;

	//���t���[���X�V
	void Update() override;

	//�`��
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	std::unique_ptr<Sprite> selectSprite_ = nullptr;
	std::unique_ptr<Sprite> BackGroundLeft = nullptr;


	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//�V�[����ς��邩�ǂ���
	bool isChangeScene_;
	
	const float kChangeSceneTime_ = 10.0f;
	float changeSceneTimer_ = kChangeSceneTime_;
};
