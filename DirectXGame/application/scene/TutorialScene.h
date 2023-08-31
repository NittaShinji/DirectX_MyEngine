#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "Sound.h"
#include "CollisionPrimitive.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
#include "LevelManager.h"
#include "GameCamera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "BaseScene.h"
#include "Stage.h"
#include "ParticleManager.h"
#include "BackGround.h"
#include "PostEffect.h"

class CollisionManager;
class TouchableObject;
class Player;

class TutorialScene : public BaseScene
{
public:

	TutorialScene();
	~TutorialScene();

	//�ÓI������
	static void StaticInitialize(DirectXBasic* directXBasic, ImGuiManager* imGuiManager);

	//������
	void Initialize() override;

	//���t���[���X�V
	void Update() override;

	//�`��
	void Draw() override;

private:

	static DirectXBasic* directXBasic_;
	static KeyInput* keys_;
	static ImGuiManager* imGuiManager_;

	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_ = nullptr;

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
	std::unique_ptr<BackGround> backGround_ = nullptr;


	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;

	//�R���C�_�[
	std::unique_ptr<SphereCollider> sphereCollider_ = nullptr;
	std::unique_ptr<TouchableObject> objGround_ = nullptr;

	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager_ = nullptr;

	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;
	std::unique_ptr<ParticleManager> playerRunEffect_ = nullptr;

	//�҂�����
	static const int32_t kWaitTime_ = 40;
	//�L�[���͂̎��ԊǗ�
	int32_t keyTimer_ = 60;

	Vector3 ambientColor0_ = { 1,1,1 };
	//�������������l
	Vector3 lightDir0_ = { 1,-5,-5 };
	Vector3 lightColor0_ = { 1,1,1 };
	Vector3 whiteColor_ = { 1,1,1 };

	float playerPosX;
	float playerPosY;
	float playerPosZ;
};

