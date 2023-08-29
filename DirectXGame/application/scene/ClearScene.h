#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include"GamePad.h"
#include "Vector2.h"

class ClearScene : public BaseScene
{

public:

	//������
	void Initialize() override;

	//���t���[���X�V
	void Update() override;

	//�`��
	void Draw() override;

	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

private:

	std::unique_ptr<Sprite> end_ = nullptr;
	std::unique_ptr<Sprite> check_ = nullptr;
	std::unique_ptr<Sprite> aButton_ = nullptr;

	static DirectXBasic* directXBasic_;
	static ImGuiManager* imGuiManager_;

	static KeyInput* keys_;
	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	Vector2 checkPosition_;

	const int32_t kActionTime_ = 60;
	int32_t moveTimer_ = kActionTime_;
	Vector2 move_;
	float rotate_;

	//1����鎞��
	const float kRotateTime_ = 30.0f;
	float rotateTimer_ = kRotateTime_;
};

