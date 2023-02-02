#pragma once

#include "DirectXBasic.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "Sound.h"
#include "CollisionPrimitive.h"
#include "ImGuiManager.h"

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
	void Initialize(DirectXBasic* directXBasic,ImGuiManager* imGuiManager);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();


private:

	//�Q�[�����̃V�[��
	enum Game
	{
		TITLE,
		GAME,
		END,
	};

	DirectXBasic* directXBasic_ = nullptr;
	static KeyInput* keys_;
	ImGuiManager* imGuiManager_ = nullptr;

	//�f�o�b�O�e�L�X�g�p�̕ϐ�
	//�E�C���h�E�̕\���t���O
	bool showEditWindow = true;
	
	//�J����
	Camera* camera_ = nullptr;
	Camera* testCamera_ = nullptr;

	//�T�E���h
	Sound* sound = nullptr;

	//�X�v���C�g
	Sprite* title_ = nullptr;
	Sprite* test_ = nullptr;


	//�X�v���C�g���ʕ���
	SpriteCommon* spriteCommon_ = nullptr;

	//���f��
	Model testModel_;

	//3D�I�u�W�F�N�g
	Object3d* object3d_ = nullptr;
	
	//�Q�[���V�[��
	int scene_;

	//�����蔻�� ��
	//Sphere sphere_;
	////�����蔻�� ����
	//Plane plane_;

	Object3d* sphere_;
	Object3d* ground_;

	//�҂�����
	static const int waitTime = 60;
	//�L�[���͂̎��ԊǗ�
	int keyTimer = 60;


};

