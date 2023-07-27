#pragma once
#include "BaseScene.h"
#include "Sprite.h"

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

	static KeyInput* keys_;
};

