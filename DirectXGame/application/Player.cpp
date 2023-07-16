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

	position_ = { 0,10,0 };
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	//���W����ݒ�
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	//playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,radius,0,0 }), radius);
	playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,0,0,0 }), radius);

	//�R���C�_�[�̓o�^
	SetCollider(playerCollider_.get());

	//collider->SetAttribute(COLLISION_ATTR_ALLIES);
	
	//�������w��
	playerCollider_->SetAttribute(COLLISION_ATTR_ALLIES);

	move = { 0,0,0 };

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
}

void Player::Update(Camera* camera)
{
	if(KeyInput::HasPushedKey(DIK_S))
	{
		position_.y += -0.05f;
		Object3d::SetTransform(position_);
		Object3d::Update(camera);
	}
	if(KeyInput::HasPushedKey(DIK_W))
	{
		position_.y += 0.05f;
		Object3d::SetTransform(position_);
		Object3d::Update(camera);
	}

	//��������
	if(!onGround)
	{
		//�����������x�@
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallVec.m128_f32[1] = max(fallVec.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		position_.x += fallVec.m128_f32[0];
		position_.y += fallVec.m128_f32[1];
		position_.z += fallVec.m128_f32[2];
	}
	//�W�����v����
	else if(keys_->HasPushedKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallVec = { 0,jumpVYFist,0,0 };
	}

	Object3d::SetTransform(position_);
	Object3d::Update(camera);

	//collider->Update();

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
	if(onGround)
	{
		//�X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		//�ڒn���ێ�
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			Object3d::SetTransform(position_);
			Object3d::Update(camera);
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else
		{
			onGround = false;
			fallVec = {0,0,0};
		}
	}
	//�������
	else if(fallVec.m128_f32[1] <= 0.0f)
	{
 		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			//���n
			onGround = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//�s��̍X�V�Ȃ�
			Object3d::SetTransform(position_);
			Object3d::Update(camera);
		}
	}

	//position_.y = 1.0f;
	//position_.x = 1.0f;

	////�s��̍X�V�Ȃ�
	//Object3d::SetTransform(position_);
	//Object3d::Update(camera);

	finish();
}

void Player::OnCollision(const CollisionInfo& info)
{
	/*onGround = true;
	fallVec = {0,0,0};*/

	/*onGround = true;
	move.y = 0;
	position_.y = 0 + radius;
	jumpCount = 2;
	isFlying_ = 0;
	jumpHeight = 0;
	Object3d::SetTransform(position_);*/
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
		isfinish_ = true;
	}*/
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


