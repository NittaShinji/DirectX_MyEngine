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
	/*Sound::GetInstance()->LoadSoundWave("jump.wav");
	Sound::GetInstance()->LoadSoundWave("doubleJump.wav");
	Sound::GetInstance()->LoadSoundWave("playerDead.wav");*/
	SoundManager::GetInstance()->LoadSoundWave("jump.wav");
	SoundManager::GetInstance()->LoadSoundWave("doubleJump.wav");
	jumpSound_ = std::make_unique<Sound>();
	doubleJumpSound_ = std::make_unique<Sound>();
	jumpSound_->Initialize("jump.wav");
	doubleJumpSound_->Initialize("doubleJump.wav");

	position_ = kPlayerInitPos_;
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	//座標情報を設定
	SetTransform(position_);
	SetRotation(rotation_);
	SetScale(scale_);

	//コライダーの追加
	float radius = 1.0f;
	playerCollider_ = std::make_unique<SphereCollider>(Vector3({ 0,0,0 }), radius);

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
	isJumpRotate_ = false;
	onGround_ = true;
	isLanded_ = false;

	fallVec_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = { 0.0f,0.0f,0.0f };
	totalAxcell_ = { 0.0f,0.0f,0.0f };

	attributeColor_ = Attribute::pink;
	colorFlag_ = true;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}

void Player::Update(Camera* camera)
{
	//どのボタンが瞬間的に押されたか
	gamePad_->PushedButtonMoment();

	//合計加速度をリセット
	totalAxcell_ = { 0.0f,0.0f,0.0f };
	if(isMoving_ == true)
	{
		totalAxcell_.z += kMoveAxcellZ_;
		Accelerate();
	}

	if(isMoving_ == true)
	{
		//色変え処理
		if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
		{
			//isScaleAnimetion_ = true;

			//属性の変更
			if(attributeColor_ == Attribute::pink)
			{
				attributeColor_ = Attribute::yellow;
			}
			else if(attributeColor_ == Attribute::yellow)
			{
				attributeColor_ = Attribute::pink;
			}
			else
			{
				attributeColor_ = Attribute::black;
			}

			colorFlag_ = true;

			if(attributeColor_ == Attribute::pink)
			{
				SetColor(Vector3(1.0f, 0.4f, 0.7f));
			}
			else if(attributeColor_ == Attribute::yellow)
			{
				SetColor(Vector3(1.0f, 0.469f, 0.0f));
			}
			else
			{
				SetColor(Vector3(0.0f, 0.0f, 0.0f));
			}
		}

		//if(isScaleAnimetion_ == true && scaleTime_)

		Object3d::SetColorFlag(colorFlag_);

		//落下処理
		if(!onGround_)
		{
			//下向き加速度　
			const float fallAcc = -0.015f;
			const float fallVYMin = -0.5f;
			//加速
			fallVec_.y = max(fallVec_.y + fallAcc, fallVYMin);

			//合計加速度に落下中の速度を加算
			totalAxcell_ += fallVec_;

			//二段ジャンプ時
			if(jumpCount > 0 && jumpCount < 2)
			{
				if(gamePad_->GetButtonA())
				{
					jumpSound_->PlaySoundWave(false);

					isJumpRotate_ = true;
					onGround_ = false;
					const float jumpVYFist = 0.4f;
					fallVec_ = { 0,jumpVYFist,0 };
					jumpCount -= 1;
				}
				if(keys_->PushedKeyMoment(DIK_SPACE))
				{
					jumpSound_->PlaySoundWave(false);
					isJumpRotate_ = true;
					onGround_ = false;
					const float jumpVYFist = 0.4f;
					fallVec_ = { 0,jumpVYFist,0 };
					jumpCount -= 1;
				}
			}

			//二段ジャンプ時の回転処理
			JumpRotation();
		}
		//ジャンプ操作
		else if(keys_->PushedKeyMoment(DIK_SPACE) && jumpCount > 0)
		{
			jumpSound_->PlaySoundWave(false);
			onGround_ = false;
			isLanded_ = false;
			const float jumpVYFist = 0.4f;
			fallVec_ = { 0,jumpVYFist,0 };
			jumpCount -= 1;
		}
		else if(gamePad_->GetButtonA() && jumpCount > 0)
		{
			jumpSound_->PlaySoundWave(false);
			onGround_ = false;
			isLanded_ = false;
			const float jumpVYFist = 0.4f;
			fallVec_ = { 0,jumpVYFist,0 };
			jumpCount -= 1;
		}
	}

	//加速を座標に反映
	position_ += totalAxcell_;
	Object3d::SetTransform(position_);
	Object3d::Update(camera);

	if(isMoving_ == true)
	{
		//球コライダーを取得
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());
		assert(sphereCollider);

		//球の上端から球の下端までのレイキャスト用レイを準備
		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.y += sphereCollider->GetRadius();
		ray.dir = { 0,-1,0 };
		RaycastHit raycastHit;

		//接地状態
		if(onGround_)
		{
			GroundRotation();

			//スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			//接地を維持
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
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
		else if(fallVec_.y <= 0.0f)
		{
			AccelerateChangeColor();

			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
			{
				//着地
				onGround_ = true;
				isLanded_ = true;
				position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = kMaxJumpNum;
				//行列の更新など
				Object3d::SetTransform(position_);
				Object3d::Update(camera);
			}
		}

		if(position_.y <= deadLine_)
		{
			//Sound::GetInstance()->PlaySoundWave("playerDead.wav", false);
			isDead_ = true;
			isLanded_ = true;
		}
	}

	//ゲームパッドのボタン情報をリセット
	gamePad_->ResetButton();
}

void Player::OnCollision(const CollisionInfo& info)
{
	if(info.object->GetAttributeColor() == Attribute::Goal)
	{
		isfinish_ = true;
	}
	//色が違う場合、死亡判定にする
	else if(info.object->GetAttributeColor() != attributeColor_ && isMoving_ == true)
	{
		//Sound::GetInstance()->PlaySoundWave("playerDead.wav", false);
		//isMoving_ = false;
		isLanded_ = false;
		isDead_ = true;
	}
}

void Player::AccelerateChangeColor()
{
	//オブジェクトと接触する際の距離を計算
	//ギリギリのところでスペースを押すと加速する

	//球コライダーを取得
	SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());
	assert(sphereCollider);

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	//下方向への方向を球1個分ずらす
	ray.start.y -= sphereCollider->GetRadius() + sphereCollider->GetRadius() / 2;
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	//スムーズに坂を下る為の吸着距離
	const float adsDistance = 0.2f;

	//地面と衝突しているかどうか
	if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
	{
		//色を変える
		if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
		{
			//加速していなかったら加速フラグを立てる
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
	//横向き加速度　
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
	if(isDead_ == false)
	{
		Object3d::Draw();
	}
}

void Player::Reset(Camera* camera)
{
	position_ = kPlayerInitPos_;
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
	isLanded_ = false;
	isChangeColor = false;
	attributeColor_ = Attribute::pink;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
	Object3d::SetTransform(position_);
	Object3d::SetRotation(rotation_);
	Object3d::Update(camera);
}

void Player::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Player");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 100));

	ImGui::SliderFloat("PlayerPosY", &position_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("PlayerPosZ", &position_.z, -100.0f, 1000.0f);

	ImGui::End();
}

void Player::JumpRotation()
{
	//地面についていなければ二段ジャンプの時に回転する
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


