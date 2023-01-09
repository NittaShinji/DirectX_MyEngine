#pragma once

#include "DirectXBasic.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Sound.h"

class GameScene
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXBasic* directXBasic);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();


private:

	DirectXBasic* directXBasic_ = nullptr;
	//static KeyInput* keys_;

	//�T�E���h
	Sound* sound = nullptr;

	//�X�v���C�g
	Sprite* title_ = nullptr;

	//�X�v���C�g���ʕ���
	SpriteCommon* spriteCommon_ = nullptr;

	//���f��
	Model testModel_;

	//3D�I�u�W�F�N�g
	Object3d* object3d_ = nullptr;
	Object3d* nObject3d_ = nullptr;
	Object3d* sObject3d_ = nullptr;

};

