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
#include "Player.h"
#include "GameCamera.h"
#include "GamePad.h"

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
	//bool showEditWindow_ = true;
	
	//���C�g
	LightGroup* lightGroup_ = nullptr;

	//�J����
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Camera> testCamera_ = nullptr;
	std::unique_ptr<GameCamera> testGameCamera_ = nullptr;


	//�T�E���h
	std::unique_ptr<Sound> sound_ = nullptr;

	//�X�v���C�g
	std::unique_ptr<Sprite> title_ = nullptr;
	std::unique_ptr<Sprite> test_ = nullptr;
	std::unique_ptr<Sprite> end_ = nullptr;


	//�v���C���[
	std::unique_ptr<Player> player_ = nullptr;
	
	//�X�v���C�g���ʕ���
	//std::unique_ptr<SpriteCommon> spriteCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	
	//�Q�[���V�[��
	int32_t scene_;

	//���x���f�[�^
	LevelData* levelData_ = nullptr;

	using MODELKEY = std::string;
	MODELKEY name_;

	std::map<MODELKEY, Model> models_;

	//�����蔻�� ��
	Sphere sphereCollision_;
	////�����蔻�� ����
	Plane planeCollision_;
	//�����蔻�� �O�p�`
	Triangle triangleCollison_;
	//�����蔻�� ���C
	Ray rayCollision_;

	//3D�I�u�W�F�N�g
	std::unique_ptr<Object3d> sphere_ = nullptr;
	std::unique_ptr<Object3d> testStage0_ = nullptr;

	//std::unique_ptr<Object3d> ground_ = nullptr;
	//std::unique_ptr<Object3d> triangle_ = nullptr;
	//std::unique_ptr<Object3d> ray_ = nullptr;

	//blender�ǂݍ��݃I�u�W�F�N�g
	std::vector<std::unique_ptr<Object3d>> objects_;

	//�摜�Ȃ��e�X�g�I�u�W�F�N�g
	std::unique_ptr<Object3d> testObject_ = nullptr;

	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//�҂�����
	static const int32_t kWaitTime_ = 40;
	//�L�[���͂̎��ԊǗ�
	int32_t keyTimer_ = 60;

	int32_t hit_;

	XMFLOAT3 move_ = { 0,0,0 };

	const int32_t kActionTime_ = 60;
	int32_t moveTimer_ = kActionTime_;
	bool isUp_ = false;
	bool isDown_ = true;

	XMFLOAT3 trianglePosition2_;
	XMFLOAT3 rayWorldPositon_;

	XMFLOAT3 rotate_ = { 0,0,0 };

	float ambientColor0_[3] = { 1,1,1 };
	//�������������l
	float lightDir0_[3] = { 0,0,1 };
	float lightColor0_[3] = { 1,0,0 };

	float lightDir1_[3] = { 0,1,0 };
	float lightColor1_[3] = { 0,1,0 };

	float lightDir2_[3] = { 1,0,0 };
	float lightColor2_[3] = { 0,0,1 };

	Input* input_ = nullptr;

	XMFLOAT3 move = { 0,0,0 };
	XMFLOAT3 sphereRotation = { 0,0,0 };

	const int actionTime = 720;
	int moveTimer = actionTime;
	bool isUp = false;
	bool isDown = true;

	//bool isStartGame_ = false;
};

