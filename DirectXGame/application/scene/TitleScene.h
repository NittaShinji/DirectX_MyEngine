#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
#include "GamePad.h"

class TitleScene : public BaseScene
{
public:

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

	std::unique_ptr<Sprite> titleSprite_ = nullptr;
	std::unique_ptr<Sprite> clickSprite_ = nullptr;
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;
	std::unique_ptr<Sprite> bButtonSprite_ = nullptr;


	std::unique_ptr<Object3d> titleSphere_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	//�҂�����
	static const int32_t waitTime = 40;

	Vector3 sphereRotate = { 0,0,0 };
	Vector3 spherPos_;

	//1����鎞��
	const float kRotateTime_ = 90.0f;
	const float kActionTime_ = 20.0f;

	const float kChangeWhiteTime_ = 10.0f;
	const float kChangeColorTime_ = 1080.0f;

	float moveTimer_ = kActionTime_;
	float rotateTimer_ = kRotateTime_;
	float changeColorTimer_ = kChangeColorTime_;
	float changeWhiteTimer_ = kChangeWhiteTime_;

	bool isUp_ = false;
	bool isDown_ = true;
	bool isChangeColor_ = true;
	bool isChangeScene_;
	Vector3 move_ = { 0,0,0 };



	float rotateAcc_;
	float moveRotate_;

	bool isJump;
	Vector3 jumpVec;
	float jumpAcc;

	//�Q�[���p�b�h
	std::unique_ptr<GamePad> gamePad_ = nullptr;
};

