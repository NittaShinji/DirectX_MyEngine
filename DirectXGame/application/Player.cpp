#include "Player.h"
#include "MathUtillity.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace std;

std::unique_ptr<Player> Player::Create(const std::string& path)
{
	//3Dオブジェクトのインスタンスを生成
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

	//座標情報を設定
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//コライダーの追加
	float radius = 1.0f;
	playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,0,0,0 }), radius);

	//コライダーの登録
	SetCollider(playerCollider_.get());
	
	//属性を指定
	playerCollider_->SetAttribute(COLLISION_ATTR_ALLIES);

	move = { 0,0,0 };

	jumpCount = 2;
	isFlying_ = 0;
	isfinish_ = false;
	isMoving_ = false;
	isChangeColor = false;
}

void Player::Update(Camera* camera)
{
	if(isMoving_ == true)
	{
		move.z = 0.3f;
		position_.z += move.z;
	}

	if(KeyInput::GetInstance()->HasPushedKey(DIK_S))
	{
		position_.z += -0.05f;
	}
	if(KeyInput::GetInstance()->HasPushedKey(DIK_W))
	{
		position_.x += 0.05f;
	}

	if(KeyInput::GetInstance()->PushedKeyMoment(DIK_O))
	{
		if(colorFlag_ == false)
		{
			colorFlag_ = true;
			SetColor(XMFLOAT3(0.0f, 0.0f, 1.0f));
		}
		else if(colorFlag_ == true)
		{
			colorFlag_ = false;
		}
	}

	Object3d::SetColorFlag(colorFlag_);

	//落下処理
	if(!onGround)
	{
		//下向き加速度　
		const float fallAcc = -0.015f;
		const float fallVYMin = -0.5f;
		//加速
		fallVec_.m128_f32[1] = max(fallVec_.m128_f32[1] + fallAcc, fallVYMin);
		//移動
		position_.x += fallVec_.m128_f32[0];
		position_.y += fallVec_.m128_f32[1];
		position_.z += fallVec_.m128_f32[2];
	}
	//ジャンプ操作
	else if(keys_->HasPushedKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFist = 0.4f;
		fallVec_ = { 0,jumpVYFist,0,0 };
	}

	Object3d::SetTransform(position_);
	Object3d::Update(camera);

	//球コライダーを取得
	SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());
	assert(sphereCollider);

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	//接地状態
	if(onGround)
	{
		//スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		//接地を維持
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			Object3d::SetTransform(position_);
			Object3d::Update(camera);
		}
		//地面がないので落下
		else
		{
			onGround = false;
			fallVec_ = {0,0,0};
		}
	}
	//落下状態
	else if(fallVec_.m128_f32[1] <= 0.0f)
	{
 		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			//着地
			onGround = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//行列の更新など
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
	if(position_.z > 235.0f)
	{
		isfinish_ = true;
	}
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
}


