#include "Player.h"
#include "MathUtillity.h"

using namespace std;

std::unique_ptr<Player> Player::Create(const std::string& path)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	std::unique_ptr<Player> instance = std::make_unique<Player>();
	if(instance == nullptr)
	{
		return nullptr;
	}

	Model::Load(path);

	instance->Initialize();
	instance->SetModel(path);

	return instance;
}

void Player::Initialize()
{
	Object3d::Initialize();

	position_ = { 0,2,0 };
	rotation_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	//���W����ݒ�
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,radius,0,0 }), radius);
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(playerCollider_.get());

	move = { 0,0,0 };

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
}

void Player::Update(Camera* camera)
{
	if(isMoving_ == true)
	{
		move.z = 0.25f;
	}

	if(jumpCount > 0)
	{
		gamePad_->HasPushedButton();
		gamePad_->ResetButton();

		gamePad_->PushedButtonMoment();

		if(gamePad_->GetButtonA())
		{
			move.y += 0.05f;
			isFlying_ = true;
			gravitySpeed = 0;
			jumpCount -= 1;
			jumpHeight = kMaxJump;
			gamePad_->ResetButton();
		}

		if(KeyInput::PushedKeyMoment(DIK_SPACE))
		{
			move.y += 0.05f;
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
			//�������̃X�s�[�h�̌v�Z
			jumpHeight -= 0.01f;
			if(jumpHeight < -kMaxJump)
			{
				//����ȏ�͗����X�s�[�h�������Ȃ�
				jumpHeight = -kMaxJump;
			}

			move.y += jumpHeight;
		}
		position_.y += move.y;
	}

	if(position_.y < 2)
	{
		position_.y = 2;
		jumpCount = 2;
		isFlying_ = 0;
		jumpHeight = 0;
		move.y = 0;
	}


	position_.z += move.z;

	Object3d::SetTransform(position_);

	Object3d::Update(camera);

	finish();
}

void Player::OnCollision(const CollisionInfo& info)
{
	move.y = +0.25f;
	position_.y += move.y;
	Object3d::SetTransform(position_);
}

void Player::SetNextState()
{

}

void Player::Draw()
{
	Object3d::Draw();
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


