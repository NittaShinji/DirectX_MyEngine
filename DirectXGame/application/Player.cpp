#include "Player.h"
#include "MathUtillity.h"

using namespace std;

void Player::Initialzie()
{
	//インスタンス生成
	object_ = std::make_unique<Object3d>();

	//モデル読み込み
	const string player = "sphere";
	Model::Load(player);

	object_->Initialize(player, scale_, position_, rotation_);

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
}

void Player::Update(Camera* camera)
{
	object_->Update(camera);

	if(isMoving_ == true)
	{
		move.z = 0.25f;
	}

	if(jumpCount > 0)
	{
		if(KeyInput::PushedKeyMoment(DIK_SPACE))
		{
			//move.y += 0.05f;
			isFlying_ = true;
			gravitySpeed = 0;
			jumpCount -= 1;
			jumpHeight = kMaxJump;
		}
	}

	if(jumpCount < 2)
	{
		if(isFlying_ == 1)
		{
			//落下中のスピードの計算
			jumpHeight -= 0.01f;
			if(jumpHeight < -kMaxJump)
			{
				//それ以上は落下スピードが落ちない
				jumpHeight = -kMaxJump;
			}

			move.y += jumpHeight;
		}
	}

	position_.y += move.y;

	if(position_.y < 2)
	{
		position_.y = 2;
		jumpCount = 2;
		isFlying_ = 0;
		jumpHeight = 0;
		move.y = 0;
	}


	position_.z += move.z;
	object_->SetTransform(position_);

	finish();
}

void Player::SetNextState()
{

}

void Player::Draw()
{
	object_->Draw();
}

void Player::finish()
{
	if(position_.z > 235.0f)
	{
		isfinish_ = true;
	}
}

void Player::Reset()
{
	position_ = { 0,2,0 };
	rotation_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	move = { 0,0,0 };

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
}


