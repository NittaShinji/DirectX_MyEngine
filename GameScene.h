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

class GameScene
{
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMMATRIX = DirectX::XMMATRIX;

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

	//�����蔻��̃`�F�b�N
	void CheckAllCollision();


private:

	enum Game
	{
		TITLE,
		GAME,
		END,
	};

	DirectXBasic* directXBasic_ = nullptr;
	KeyInput* keys_;

	//�J����
	Camera* camera_ = nullptr;
	Camera* testCamera_ = nullptr;

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

	//�Q�[���V�[��
	int scene_;

	//AABB�̏Փ˔���t���O
	bool AABBisHit_;

};

