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

	//ジャンプカウント
	int32_t jumpCount;

	// 等速ジャンプの1フレームごとのジャンプの上がり幅
	float jumpHeight;

	//jumpheightの最大値
	const float kMaxJump = 0.1f;

	//空中にいるかどうか
	bool isFlying_;

	//重力
	float gravitySpeed = 0.0f;

	//動いているか
	bool isMoving_;

	//ゴールしたかどうか
	bool isfinish_;

public:

	bool GetIsMoving() { return isMoving_; }
	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }

	bool GetIsFinish() { return isfinish_; }


};

