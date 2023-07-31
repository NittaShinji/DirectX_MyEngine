#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include"GamePad.h"

class ClearScene : public BaseScene
{

public:

	//������
	void Initialize() override;

	//���t���[���X�V
	void Update() override;

	//�`��
	void Draw() override;

private:

	std::unique_ptr<Sprite> end_ = nullptr;
	std::unique_ptr<Sprite> check_ = nullptr;
	std::unique_ptr<Sprite> aButton_ = nullptr;

	static KeyInput* keys_;
	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	XMFLOAT2 checkPosition_;

	const int32_t kActionTime_ = 60;
	int32_t moveTimer_ = kActionTime_;
	XMFLOAT2 move_;
};

