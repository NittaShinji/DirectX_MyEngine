#pragma once
#include "BaseScene.h"
#include "Sprite.h"

class TitleScene : public BaseScene
{
public:

	static void StaticInitialize();

	//������
	void Initialize() override;

	//���t���[���X�V
	void Update() override;

	//�I��
	void Finalize() override;

	//�`��
	void Draw() override;

private:

	std::unique_ptr<Sprite> title_ = nullptr;

	static KeyInput* keys_;
};

