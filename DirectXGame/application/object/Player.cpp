#include "Player.h"
#include "MathUtillity.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Sound.h"
#include "Easing.h"

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
	Sound::GetInstance()->LoadSoundWave("jump.wav");

	position_ = { 0,2,2 };
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	//���W����ݒ�
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	playerCollider_ = std::make_unique<SphereCollider>(Vector3({ 0,0,0}), radius);


	//�R���C�_�[�̓o�^
	SetCollider(playerCollider_.get());

	//�������w��
	playerCollider_->SetAttribute(COLLISION_ATTR_ALLIES);

	move = { 0,0,0 };

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
	isChangeColor = false;

	attribute_ = Attribute::pink;
	colorFlag_ = true;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}

void Player::Update(Camera* camera)
{
	if(isMoving_ == true)
	{
		move.z = 0.5f;
		position_.z += move.z;
	}

	gamePad_->PushedButtonMoment();
	
	if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
	{
		gamePad_->ResetButton();
		//�����̕ύX
		if(attribute_ == Attribute::pink)
		{
			attribute_ = Attribute::yellow;
		}
		else if(attribute_ == Attribute::yellow)
		{
			attribute_ = Attribute::pink;
		}
		else
		{
			attribute_ = Attribute::black;
		}
		
		colorFlag_ = true;

		if(attribute_ == Attribute::pink)
		{
			SetColor(Vector3(1.0f, 0.4f, 0.7f));
		}
		else if(attribute_ == Attribute::yellow)
		{
			SetColor(Vector3(1.0f, 0.469f, 0.0f));
		}
		else
		{
			SetColor(Vector3(0.0f, 0.0f, 0.0f));
		}
	}

	Object3d::SetColorFlag(colorFlag_);

	//��������
	if(!onGround_)
	{
		//�����������x�@
		const float fallAcc = -0.015f;
		const float fallVYMin = -0.5f;
		//����
		fallVec_.y = max(fallVec_.y + fallAcc, fallVYMin);
		//�ړ�
		position_.x += fallVec_.x;
		position_.y += fallVec_.y;
		position_.z += fallVec_.z;

		if(jumpCount > 0)
		{
			if(gamePad_->GetButtonA())
			{
				gamePad_->ResetButton();
				Sound::GetInstance()->PlaySoundWave("jump.wav");
				onGround_ = false;
				const float jumpVYFist = 0.4f;
				fallVec_ = { 0,jumpVYFist,0};
				jumpCount -= 1;
			}
			if(keys_->PushedKeyMoment(DIK_SPACE))
			{
				onGround_ = false;
				const float jumpVYFist = 0.4f;
				fallVec_ = { 0,jumpVYFist,0 };
				jumpCount -= 1;
			}	
		}
	}
	//�W�����v����
	else if(keys_->PushedKeyMoment(DIK_SPACE) && jumpCount > 0)
	{
		Sound::GetInstance()->PlaySoundWave("jump.wav");
		onGround_ = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0};
		jumpCount -= 1;
	}
	else if(gamePad_->GetButtonA() && jumpCount > 0)
	{
		Sound::GetInstance()->PlaySoundWave("jump.wav");
		gamePad_->ResetButton();
		onGround_ = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0};
		jumpCount -= 1;
	}

	Object3d::SetTransform(position_);
	Object3d::Update(camera);

	//���R���C�_�[���擾
	SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());
	assert(sphereCollider);

	//���̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.y += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	//�ڒn���
	if(onGround_)
	{
		
		//�X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		//�ڒn���ێ�
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround_ = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			Object3d::SetTransform(position_);
			Object3d::Update(camera);
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else
		{
			onGround_ = false;
			fallVec_ = { 0,0,0 };
		}
	}
	//�������
	else if(fallVec_.y <= 0.0f)
	{
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			//���n
			onGround_ = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			jumpCount = kMaxJumpNum;
			//�s��̍X�V�Ȃ�
			Object3d::SetTransform(position_);
			Object3d::Update(camera);
		}
	}

	if(position_.y <= -5)
	{
		isDead_ = true;
	}

	finish();
}

void Player::OnCollision(const CollisionInfo& info)
{
	//�F���Ⴄ�ꍇ�A���S����ɂ���
	if(info.object->GetAttribute() != attribute_)
	{
		isDead_ = true;
	}

	if(info.object->GetAttribute() == Attribute::Goal)
	{
		isfinish_ = true;
	}
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
	/*if(position_.z > 235.0f)
	{
		isDead_ = true;
	}*/
}

void Player::Reset()
{
	position_ = { 0,2,2 };
	rotation_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	move = { 0,0,0 };

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
	isDead_ = false;
	isChangeColor = false;
	attribute_ = Attribute::pink;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}


