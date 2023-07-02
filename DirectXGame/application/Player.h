#pragma once
#include"Object3d.h"
#include "Model.h"
#include "Camera.h"

class Player
{

public:

	void Initialzie();

	void Update(Camera* camera);

	void SetNextState();

	void Draw();

	void finish();

	void Reset();

private:

	std::unique_ptr<Object3d> object_;

	XMFLOAT3 position_ = { 0,2,0 };
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

public:

	bool GetIsMoving() { return isMoving_; }
	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }

	bool GetIsFinish() { return isfinish_; }


};

