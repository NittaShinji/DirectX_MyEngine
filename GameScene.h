#pragma once

#include "DirectXBasic.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"

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
	Input* input_ = nullptr;

	//���f��
	Model testModel;

	//3D�I�u�W�F�N�g
	Object3d* object3d = nullptr;
	Object3d* nObject3d = nullptr;

	//�X�v���C�g
	

};

