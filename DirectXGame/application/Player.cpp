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

	position_ = { 0,10,0 };
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	//座標情報を設定
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//コライダーの追加
	float radius = 1.0f;
	//playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,radius,0,0 }), radius);
	playerCollider_ = std::make_unique<SphereCollider>(XMVECTOR({ 0,0,0,0 }), radius);

	//コライダーの登録
	SetCollider(playerCollider_.get());

	//collider->SetAttribute(COLLISION_ATTR_ALLIES);
	
	//属性を指定
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

	//落下処理
	if(!onGround)
	{
		//下向き加速度　
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//加速
		fallVec.m128_f32[1] = max(fallVec.m128_f32[1] + fallAcc, fallVYMin);
		//移動
		position_.x += fallVec.m128_f32[0];
		position_.y += fallVec.m128_f32[1];
		position_.z += fallVec.m128_f32[2];
	}
	//ジャンプ操作
	else if(keys_->HasPushedKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallVec = { 0,jumpVYFist,0,0 };
	}

	Object3d::SetTransform(position_);
	Object3d::Update(camera);

	//collider->Update();

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
			fallVec = {0,0,0};
		}
	}
	//落下状態
	else if(fallVec.m128_f32[1] <= 0.0f)
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

	//position_.y = 1.0f;
	//position_.x = 1.0f;

	////行列の更新など
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


