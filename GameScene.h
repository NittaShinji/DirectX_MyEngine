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
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "LevelManager.h"

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
	
	//���C�g
	LightGroup* lightGroup_ = nullptr;

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
	//Object3d* object3d_ = nullptr;
	//std::vector<Object3d*> objects;
	//std::vector<std::unique_ptr<Object3d>> objects;

	//�摜�Ȃ��e�X�g�I�u�W�F�N�g
	Object3d* testObject = nullptr;
	
	//�Q�[���V�[��
	int scene_;

	//���x���f�[�^
	//LevelManager* levelManager_ = nullptr;
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;

	MODELKEY name_;

	std::map<MODELKEY, Model> models_;

	//�����蔻�� ��
	Sphere sphereCollision;
	////�����蔻�� ����
	Plane planeCollision;
	//�����蔻�� �O�p�`
	Triangle triangleCollison;
	//�����蔻�� ���C
	Ray rayCollision;

	Object3d* sphere_;
	Object3d* ground_;
	Object3d* triangle_;
	Object3d* ray_;

	Object3d* blender_;

	//�҂�����
	static const int waitTime = 40;
	//�L�[���͂̎��ԊǗ�
	int keyTimer = 60;

	int hit;

	XMFLOAT3 move = { 0,0,0 };

	const int actionTime = 60;
	int moveTimer = actionTime;
	bool isUp = false;
	bool isDown = true;

	XMFLOAT3 trianglePosition2;
	XMFLOAT3 rayWorldPositon;

	XMFLOAT3 rotate = { 0,0,0 };

	float ambientColor0[3] = { 1,1,1 };
	//�������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

};

