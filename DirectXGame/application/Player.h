#pragma once
#include"Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "GamePad.h"
#include "SphereCollider.h"

class Player : public Object3d
{
private:

	//nameSpace
	using XMFLOAT = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static std::unique_ptr<Player> Create(const std::string& path);

public:

	void Initialize() override;

	void Update(Camera* camera) override;

	void OnCollision(const CollisionInfo& info) override;

	void SetNextState();

	void Draw();

	void finish();

	void Reset();

private:

	std::unique_ptr<Object3d> object_;

	XMFLOAT3 position_ = { 0,2,0 };
	//XMMATRIX position_ = { 0,2,0 };
	XMFLOAT3 rotation_ = { 0,0,0 };
	XMFLOAT3 scale_ = { 1,1,1 };

	XMFLOAT3 move = { 0,0,0 };

	//�W�����v�J�E���g
	int32_t jumpCount;

	// �����W�����v��1�t���[�����Ƃ̃W�����v�̏オ�蕝
	float jumpHeight;

	//jumpheight�̍ő�l
	const float kMaxJump = 0.1f;

	//�󒆂ɂ��邩�ǂ���
	bool isFlying_;

	//�d��
	float gravitySpeed = 0.0f;

	//�����Ă��邩
	bool isMoving_;

	//�S�[���������ǂ���
	bool isfinish_;

	//���a
	float radius = 0.6f;

	//�v���C���[�̃R���C�_�[
	std::unique_ptr<SphereCollider> playerCollider_ = nullptr;

	//�v���C���[�p�̃R���g���[���[
	//std::unique_ptr<GamePad> player_ = nullptr;
	GamePad* gamePad_ = nullptr;

	//�ڒn�t���O
	bool onGround = false;

	//���S�t���O
	bool isDead_ = false;

	//�����x�N�g��
	DirectX::XMVECTOR fallVec_;

	bool isChangeColor = false;

	const int32_t pushTime = 1;

	int32_t colorTimer = pushTime;

	bool pushMoment_ = false;

public:

	bool GetIsMoving() { return isMoving_; }
	bool GetIsFinish() { return isfinish_; }
	bool GetIsDead() { return isDead_; }

	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }
	void SetGamePad(GamePad* gamePad) { gamePad_ = gamePad; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }



};

