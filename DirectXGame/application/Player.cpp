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
		//属性の変更
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

	//落下処理
	if(!onGround_)
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
	//ジャンプ操作
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
	if(onGround_)
	{
		
		//スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		//接地を維持
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround_ = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			Object3d::SetTransform(position_);
			Object3d::Update(camera);
		}
		//地面がないので落下
		else
		{
			onGround_ = false;
			fallVec_ = { 0,0,0 };
		}
	}
	//落下状態
	else if(fallVec_.m128_f32[1] <= 0.0f)
	{
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			//着地
			onGround_ = true;
			position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			jumpCount = kMaxJumpNum;
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
	//色が違う場合、死亡判定にする
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


