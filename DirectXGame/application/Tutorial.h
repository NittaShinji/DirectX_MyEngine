#pragma once
#include "Camera.h"

class Tutorial
{
public:

	Tutorial();
	~Tutorial();

	//������
	void Initialize();

	//���t���[���X�V
	void Update();

	//�`��
	void Draw();

private:

	//�X�v���C�g�����t���O
	bool isShowSpriteA;

};

