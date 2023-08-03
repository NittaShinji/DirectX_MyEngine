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
#include "GameCamera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "BaseScene.h"
#include "Stage.h"
#include "ParticleManager.h"

class CollisionManager;
class TouchableObject;
class Player;

class GameScene : public BaseScene
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

	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;
	
	//���C�g
	LightGroup* lightGroup_ = nullptr;

	//�J����
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Camera> testCamera_ = nullptr;
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	//�v���C���[
	std::unique_ptr<Player> player_ = nullptr;

	//3D�I�u�W�F�N�g
	std::unique_ptr<Stage> stage_ = nullptr;

	//�w�i
	std::unique_ptr<Object3d> skydome_ = nullptr;

	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//�R���C�_�[
	std::unique_ptr<SphereCollider> sphereCollider_ = nullptr;
	std::unique_ptr<TouchableObject> objGround_ = nullptr;

	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager_ = nullptr;

	//�҂�����
	static const int32_t kWaitTime_ = 40;
	//�L�[���͂̎��ԊǗ�
	int32_t keyTimer_ = 60;

	//float ambientColor0_[3] = { 1,1,1 };
	Vector3 ambientColor0_ = { 1,1,1 };
	//�������������l
	//float lightDir0_[3] = { 1,-5,-5 };
	Vector3 lightDir0_ = { 1,-5,-5 };

	//float lightColor0_[3] = { 1,1,1 };
	Vector3 lightColor0_ = { 1,1,1 };

	//float whiteColor_[3] = { 1,1,1 };
	Vector3 whiteColor_ = { 1,1,1 };

	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;
	std::unique_ptr<ParticleManager> playerRunEffect_ = nullptr;
};

