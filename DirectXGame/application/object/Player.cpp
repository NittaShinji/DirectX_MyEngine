#include "Player.h"
#include "MathUtillity.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Sound.h"
#include "Easing.h"
#include "ImGuiManager.h"

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
	/*Sound::GetInstance()->LoadSoundWave("jump.wav");
	Sound::GetInstance()->LoadSoundWave("doubleJump.wav");
	Sound::GetInstance()->LoadSoundWave("playerDead.wav");*/

	playerInitPos_ = { 0.0f,2.0f,2.0f };
	position_ = playerInitPos_;
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	//���W����ݒ�
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	playerCollider_ = std::make_unique<SphereCollider>(Vector3({ 0,0,0 }), radius);

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
	isJumpRotate_ = false;

	fallVec_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = { 0.0f,0.0f,0.0f };
	totalAxcell_ = { 0.0f,0.0f,0.0f };

	attribute_ = Attribute::pink;
	colorFlag_ = true;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}

void Player::Update(Camera* camera)
{
	//�ǂ̃{�^�����u�ԓI�ɉ����ꂽ��
	gamePad_->PushedButtonMoment();

	//���v�����x�����Z�b�g
	totalAxcell_ = { 0.0f,0.0f,0.0f };
	if(isMoving_ == true)
	{
		totalAxcell_.z += kMoveAxcellZ_;
		Accelerate();
	}

	//�F�ς�����
	if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
	{
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

		//���v�����x�ɗ������̑��x�����Z
		totalAxcell_ += fallVec_;

		//��i�W�����v��
		if(jumpCount > 0 && jumpCount < 2)
		{
			if(gamePad_->GetButtonA())
			{
				//Sound::GetInstance()->PlaySoundWave("doubleJump.wav", false);
				isJumpRotate_ = true;
				onGround_ = false;
				const float jumpVYFist = 0.4f;
				fallVec_ = { 0,jumpVYFist,0 };
				jumpCount -= 1;
			}
			if(keys_->PushedKeyMoment(DIK_SPACE))
			{
				//Sound::GetInstance()->PlaySoundWave("doubleJump.wav", false);
				isJumpRotate_ = true;
				onGround_ = false;
				const float jumpVYFist = 0.4f;
				fallVec_ = { 0,jumpVYFist,0 };
				jumpCount -= 1;
			}
		}

		//��i�W�����v���̉�]����
		JumpRotation();
	}
	//�W�����v����
	else if(keys_->PushedKeyMoment(DIK_SPACE) && jumpCount > 0)
	{
		//Sound::GetInstance()->PlaySoundWave("jump.wav", false);
		onGround_ = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0 };
		jumpCount -= 1;
	}
	else if(gamePad_->GetButtonA() && jumpCount > 0)
	{
		//Sound::GetInstance()->PlaySoundWave("jump.wav", false);
		onGround_ = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0 };
		jumpCount -= 1;
	}

	//���������W�ɔ��f
	position_ += totalAxcell_;
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
		GroundRotation();

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
		AccelerateChangeColor(camera_);

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

	if(position_.y <= deadLine_)
	{
		//Sound::GetInstance()->PlaySoundWave("playerDead.wav", false);
		isDead_ = true;
	}

	//�Q�[���p�b�h�̃{�^���������Z�b�g
	gamePad_->ResetButton();
}

void Player::OnCollision(const CollisionInfo& info)
{
	//�F���Ⴄ�ꍇ�A���S����ɂ���
	if(info.object->GetAttribute() != attribute_)
	{
		//Sound::GetInstance()->PlaySoundWave("playerDead.wav", false);
		isDead_ = true;
	}

	if(info.object->GetAttribute() == Attribute::Goal)
	{
		isfinish_ = true;
	}
}

void Player::AccelerateChangeColor(Camera* camera)
{
	//�I�u�W�F�N�g�ƐڐG����ۂ̋������v�Z
	//�M���M���̂Ƃ���ŃX�y�[�X�������Ɖ�������

	//���R���C�_�[���擾
	SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());
	assert(sphereCollider);

	//���̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	//�������ւ̕�������1�����炷
	ray.start.y -= sphereCollider->GetRadius() + sphereCollider->GetRadius() / 2;
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	//�X���[�Y�ɍ������ׂ̋z������
	const float adsDistance = 0.2f;

	//�n�ʂƏՓ˂��Ă��邩�ǂ���
	if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
	{
		//�F��ς���
		if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
		{
			//�������Ă��Ȃ�����������t���O�𗧂Ă�
			if(isRightAxcell_ == false)
			{
				isRightAxcell_ = true;
				axcellTimer_ = kAxcellTime_;
			}
		}
	}
}

void Player::Accelerate()
{
	//�����������x�@
	const float rightAcc = 0.015f;
	const float rightVZMin = 0.3f;

	if(isRightAxcell_ == true)
	{
		axcellTimer_--;
		if(axcellTimer_ > 0)
		{
			rightAxcellVec_.z = max(rightAxcellVec_.z + rightAcc, rightVZMin);
		}
		else
		{
			rightAxcellVec_.z = 0;
			isRightAxcell_ = false;
		}
	}

	totalAxcell_.z += rightAxcellVec_.z;
}

void Player::Draw()
{
	Object3d::Draw();
}

void Player::Reset()
{
	position_ = { 0,2,2 };
	rotation_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	move = { 0,0,0 };

	jumpCount = 2;
	onGround_ = true;
	isJumpRotate_ = false;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
	isDead_ = false;
	isChangeColor = false;
	attribute_ = Attribute::pink;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}

void Player::ImGuiUpdate()
{
	//�X�v���C�g�̕ҏW�E�C���h�E�̕\��

	ImGui::Begin("Player");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 100));

	ImGui::SliderFloat("PlayerPosY", &position_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("PlayerPosZ", &position_.z, -100.0f, 1000.0f);

	ImGui::End();
}

void Player::JumpRotation()
{
	//�n�ʂɂ��Ă��Ȃ���Γ�i�W�����v�̎��ɉ�]����
	if(isJumpRotate_ == true)
	{
		float angle = ToRadian(360.0f);
		rotation_.x -= PlayEaseIn(rotateXTimer_, 0.0f, angle, kRotateXTime_);

		if(rotateXTimer_ >= 0)
		{
			rotateXTimer_--;
		}
		else
		{
			rotateXTimer_ = kRotateXTime_;
			isJumpRotate_ = false;
		}
	}

	Object3d::SetRotation(rotation_);
}

void Player::GroundRotation()
{

	float angle = ToRadian(360.0f);
	float rotateYVec = 0.10f;

	if(rotateYTimer_ >= 0)
	{
		rotateYTimer_ -= rotateYVec;
		float angle = ToRadian(rotateYTimer_);
		rotation_.y -= angle;
	}
	else
	{
		rotateYTimer_ = kRotateYTime_;
		isJumpRotate_ = false;
	}


	Object3d::SetRotation(rotation_);
}


