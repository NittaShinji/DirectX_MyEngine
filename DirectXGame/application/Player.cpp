#include "Player.h"
#include "MathUtillity.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

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

	position_ = { 0,2,2 };
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	//���W����ݒ�
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,0,0,0 }), radius);

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
	SetColor(XMFLOAT3(1.0f, 0.4f, 0.7f));
}

void Player::Update(Camera* camera)
{
	if(isMoving_ == true)
	{
		move.z = 0.5f;
		position_.z += move.z;
	}

	gamePad_->PushedButtonMoment();
	
	if(KeyInput::GetInstance()->PushedKeyMoment(DIK_O) || gamePad_->GetButtonB())
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
			SetColor(XMFLOAT3(1.0f, 0.4f, 0.7f));
		}
		else if(attribute_ == Attribute::yellow)
		{
			SetColor(XMFLOAT3(1.0f, 0.469f, 0.0f));
		}
		else
		{
			SetColor(XMFLOAT3(0.0f, 0.0f, 0.0f));
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
		fallVec_.m128_f32[1] = max(fallVec_.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		position_.x += fallVec_.m128_f32[0];
		position_.y += fallVec_.m128_f32[1];
		position_.z += fallVec_.m128_f32[2];

		if(jumpCount > 0)
		{
			if(gamePad_->GetButtonA())
			{
				gamePad_->ResetButton();
				onGround_ = false;
				const float jumpVYFist = 0.4f;
				fallVec_ = { 0,jumpVYFist,0,0 };
				jumpCount -= 1;
			}
			/*if(keys_->PushedKeyMoment(DIK_SPACE))
			{
				onGround_ = false;
				const float jumpVYFist = 0.4f;
				fallVec_ = { 0,jumpVYFist,0,0 };
				jumpCount -= 1;
			}	*/
		}
	}
	//�W�����v����
	/*else if(keys_->PushedKeyMoment(DIK_SPACE) && jumpCount > 0)
	{
		onGround_ = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0,0 };
		jumpCount -= 1;
	}*/
	else if(gamePad_->GetButtonA() && jumpCount > 0)
	{
		gamePad_->ResetButton();
		onGround_ = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0,0 };
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
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
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
	else if(fallVec_.m128_f32[1] <= 0.0f)
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
	SetColor(XMFLOAT3(1.0f, 0.4f, 0.7f));
}


