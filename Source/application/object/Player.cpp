#include "Player.h"
#include "MathUtillity.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Sound.h"
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

	/*jumpEasing_.endDistance = 0.0f;
	jumpEasing_.startPos = 0.0f;
	jumpEasing_.totalTime = 60.0f;
	jumpEasing_.time = 0.0f;

	LandEasing_ = { 60.0f,0.0f,0.0f,0.0f };
*/

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
	isStartedJumpAnimation_ = false;
	isStartedLandAnime_ = false;
	isReturnedSizeAnime_ = false;
	isReadyToJump_ = false;
	isDuringAnimation_ = false;

	fallVec_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = { 0.0f,0.0f,0.0f };
	totalAxcell_ = { 0.0f,0.0f,0.0f };

	attributeColor_ = Attribute::pink;
	colorFlag_ = true;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}

void Player::Update(Camera* camera)
{
	gameCamera_ = camera;

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
			isStartChangeColorAnime_ = true;

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

		Object3d::SetColorFlag(colorFlag_);

		if(isStartChangeColorAnime_ == true && isStartedLandAnime_ == false && isReturnedSizeAnime_ == false && isDentedAnime_ == false)
		{
			ColorChangeAnimation();
		}
		else
		{
			if(isStartedLandAnime_ == false && isReturnedSizeAnime_ == false && isDentedAnime_ == false)
			{
				scale_ = { 1.0f,1.0f,1.0f };
				//scale_ = kMoveScale;

			}
			//.SetScale(scale_);
		}

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

					isTouchUnderObject_ = false;
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
					isTouchUnderObject_ = false;
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
			/*onGround_ = false;
			isLanded_ = false;*/
			isDentedAnime_ = true;
			onGround_ = false;
			isLanded_ = false;
			const float jumpVYFist = 0.4f;
			fallVec_ = { 0,jumpVYFist,0 };
			jumpCount -= 1;
			isReadyToJump_ = false;

		}
		else if(gamePad_->GetButtonA() && jumpCount > 0)
		{
			jumpSound_->PlaySoundWave(false);
			isDentedAnime_ = true;
		}
	}

	if(isStartedJumpAnimation_ == true)
	{
		if(jumpAnimationTimer_ > 0)
		{
			jumpAnimationTimer_--;
		}
		else
		{
			jumpAnimationTimer_ = kJumpAnimationTime_;
			isStartedJumpAnimation_ = false;
		}
	}

	//加速を座標に反映
	position_ += totalAxcell_;
	Object3d::SetTransform(position_);
	UpdateWorldMatrix();
	collider_->Update();

	if(isMoving_ == true)
	{
		//球コライダーを取得
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());
		assert(sphereCollider);

		//クエリ―コールバッククラス
		class PlayerQueryCallback : public QueryCallback
		{
		public:
			PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

			//衝突時コールバック関数
			bool OnQueryHit(const QueryHit& info)
			{
				//ワールドの上方向
				const Vector3 up = { 0,1,0 };
				//排斥方向
				Vector3 rejectDir = Vector3Normalize(info.reject);
				//上方向と排斥方向の角度差のコサイン値
				float cos = Vector3Dot(rejectDir, up).x;

				//地面判定しきい値角度
				const float threshold = cosf(ToRadian(60.0f));
				//角度差によって天井または地面判定される場合を除いて
				if(-threshold < cos && cos < threshold)
				{
					//球を排斥(押し出す)
					sphere->center += info.reject;
					move += info.reject;
				}

				return true;
			}

			//クエリ―に使用する球
			Sphere* sphere = nullptr;
			//排斥による移動量(累積値)
			Vector3 move = {};
		};

		//クエリーコールバックの関数オブジェクト
		PlayerQueryCallback callback(sphereCollider);

		//球と地形の交差を全検索
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
		//交差による排斥分動かす
		if(onGround_ == false)
		{
			position_.x += callback.move.x;
			position_.y += callback.move.y;
			position_.z += callback.move.z;
			//押し出し時
			if(callback.move.z < 0)
			{
				isStoped_ = true;
			}
			else
			{
				isStoped_ = false;
			}	
		}

		//コライダー更新
		Object3d::UpdateWorldMatrix();
		collider_->Update();

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
				//Object3d::SetScale(kMoveScale);
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
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				isStartedLandAnime_ = true;
				position_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = kMaxJumpNum;
				//行列の更新など
				Object3d::SetTransform(position_);
				//Object3d::SetScale(kMaxLandMomentScale);
			}
		}
		
		//落下処理
		if(position_.y <= deadLine_)
		{
			//Sound::GetInstance()->PlaySoundWave("playerDead.wav", false);
			isDead_ = true;
			isLanded_ = true;
		}
	}

	if(isStartedLandAnime_ == true)
	{
		Animation(isStartedLandAnime_, kLandScaleSpeed_, kMaxLandMomentScale_);
		if(isDuringAnimation_ == false)
		{
			isReturnedSizeAnime_ = true;
			isStartedLandAnime_ = false;
		}
	}
	else if(isReturnedSizeAnime_ == true)
	{
		Animation(isReturnedSizeAnime_,kReturnScaleSpeed_,kMoveScale_);
		if(isDuringAnimation_ == false)
		{
			isReturnedSizeAnime_ = false;
		}
	}
	else if(isDentedAnime_ == true)
	{
		Animation(isDentedAnime_, kDentSpeed_, kDentedScale_);
	}

	Object3d::Update(camera);

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
		isLanded_ = false;
		isDead_ = true;
	}

	//プレイヤーと同じ色の場合
	if(info.object->GetAttributeColor() == attributeColor_)
	{
		////球コライダーを取得
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());

		Vector3 objectPosition = info.object->GetTransform();
		if(position_.y + sphereCollider->GetRadius() <= objectPosition.y && isTouchUnderObject_ == false)
		{
			isTouchUnderObject_ = true;
			position_ -= totalAxcell_;

			//加速
			fallVec_.y = 0.0f;
		}
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
	const float rightAcc = 0.04f;
	const float rightVZMin = 0.7f;

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
	isStartedLandAnime_ = false;
	isReturnedSizeAnime_ = false;
	isStartedJumpAnimation_ = false;
	isReadyToJump_ = false;
	isDentedAnime_ = false;

	attributeColor_ = Attribute::pink;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
	Object3d::SetTransform(position_);
	Object3d::SetRotation(rotation_);
	Object3d::SetScale(scale_);
	Object3d::Update(camera);
}

void Player::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Player");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 200));

	ImGui::SliderFloat("PlayerPosY", &position_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("PlayerPosZ", &position_.z, -100.0f, 1000.0f);
	ImGui::SliderFloat("PlayerScaleX", &scale_.x, 0.0f, 5.0f);
	ImGui::SliderFloat("PlayerScaleY", &scale_.y, 0.0f, 5.0f);
	ImGui::SliderFloat("PlayerScaleZ", &scale_.z, 0.0f, 5.0f);
	Object3d::SetScale(scale_);

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

void Player::ColorChangeAnimation()
{
	if(scaleTime_ > 0)
	{
		scaleTime_--;
	}
	else
	{
		scaleTime_ = kScaleTime_;
		isStartChangeColorAnime_ = false;
	}

	if(isStartChangeColorAnime_ == true)
	{
		scale_.x = PlayEaseIn(scaleTime_, 1.0f, 1.25f, kScaleTime_);
		scale_.y = PlayEaseIn(scaleTime_, 1.0f, 1.25f, kScaleTime_);
		scale_.z = PlayEaseIn(scaleTime_, 1.0f, 1.25f, kScaleTime_);
		Object3d::SetScale(scale_);
	}
}

void Player::LandScaleAnimation()
{
	/*if(isStartedLandAnime_ == true)
	{
		bool isExpandingX = true;
		if(scale_.x <= kMaxLandMomentScale_.x)
		{
			scale_.x += kLandScaleSpeed_;
		}
		else
		{
			isExpandingX = false;
		}

		bool isExpandingY = true;
		if(scale_.y >= kMaxLandMomentScale_.y)
		{
			scale_.y -= kLandScaleSpeed_;
		}
		else
		{
			isExpandingY = false;
		}

		bool isExpandingZ = true;
		if(scale_.z <= kMaxLandMomentScale_.z)
		{
			scale_.z += kLandScaleSpeed_;
		}
		else
		{
			isExpandingZ = false;
		}

		if(isExpandingX == false && isExpandingY == false && isExpandingZ == false)
		{
			isStartedLandAnime_ = false;
			isReturnedSizeAnime_ = true;
		}

		Object3d::SetScale(scale_);
	}*/

	if(isStartedLandAnime_ == true)
	{
		Animation(isStartedLandAnime_, kLandScaleSpeed_, kMaxLandMomentScale_);	
	}
}

void Player::ReturnMoveAnimation()
{
	if(isReturnedSizeAnime_ == true)
	{
		bool isExpandingX = true;
		if(scale_.x <= kMoveScale_.x && isExpandingX == true)
		{
			scale_.x += kReturnScaleSpeed_;
			if(scale_.x >= kMoveScale_.x)
			{
				isExpandingX = false;
			}
		}
		else if(scale_.x > kMoveScale_.x && isExpandingX == true)
		{
			scale_.x -= kReturnScaleSpeed_;
			if(scale_.x <= kMoveScale_.x)
			{
				isExpandingX = false;
			}
		}

		bool isExpandingY = true;
		if(scale_.y >= kMoveScale_.y && isExpandingY == true)
		{
			scale_.y -= kReturnScaleSpeed_;
			if(scale_.x <= kMoveScale_.x)
			{
				isExpandingY = false;
			}
		}
		else if(scale_.y < kMoveScale_.y && isExpandingY == true)
		{
			scale_.y += kReturnScaleSpeed_;
			if(scale_.y >= kMoveScale_.y)
			{
				isExpandingY = false;
			}
		}
		
		bool isExpandingZ = true;
		if(scale_.z <= kMoveScale_.z && isExpandingZ == true)
		{
			scale_.z += kReturnScaleSpeed_;
			if(scale_.z >= kMoveScale_.z)
			{
				isExpandingZ = false;
			}
		}
		else if(scale_.z > kMoveScale_.z && isExpandingZ == true)
		{
			scale_.z -= kReturnScaleSpeed_;
			if(scale_.z <= kMoveScale_.z)
			{
				isExpandingZ = false;
			}
		}

		if(isExpandingX == false && isExpandingY == false && isExpandingZ == false)
		{
			isReturnedSizeAnime_ = false;
		}

		Object3d::SetScale(scale_);
	}
}

void Player::ReadyToJumpAnimation()
{
	if(isDentedAnime_ == true)
	{
		bool isExpandingX = true;
		if(scale_.x <= kDentedScale_.x && isExpandingX == true)
		{
			scale_.x += kDentSpeed_;
			if(scale_.x >= kDentedScale_.x)
			{
				isExpandingX = false;
			}
		}
		else if(scale_.x > kDentedScale_.x && isExpandingX == true)
		{
			scale_.x -= kDentSpeed_;
			if(scale_.x <= kDentedScale_.x)
			{
				isExpandingX = false;
			}
		}

		bool isExpandingY = true;
		if(scale_.y >= kDentedScale_.y && isExpandingY == true)
		{
			scale_.y -= kDentSpeed_;
			if(scale_.x <= kDentedScale_.x)
			{
				isExpandingY = false;
			}
		}
		else if(scale_.y < kDentedScale_.y && isExpandingY == true)
		{
			scale_.y += kDentSpeed_;
			if(scale_.y >= kDentedScale_.y)
			{
				isExpandingY = false;
			}
		}

		bool isExpandingZ = true;
		if(scale_.z <= kDentedScale_.z && isExpandingZ == true)
		{
			scale_.z += kDentSpeed_;
			if(scale_.z >= kDentedScale_.z)
			{
				isExpandingZ = false;
			}
		}
		else if(scale_.z > kDentedScale_.z && isExpandingZ == true)
		{
			scale_.z -= kDentSpeed_;
			if(scale_.z <= kDentedScale_.z)
			{
				isExpandingZ = false;
			}
		}

		if(isExpandingX == false && isExpandingY == false && isExpandingZ == false)
		{
			isDentedAnime_ = false;
			isReadyToJump_ = true;
			isReturnedSizeAnime_ = true;
		}
	}

}

void Player::Animation(bool isStartedAnime, float animationSpeed, Vector3 goalScale)
{
	if(isStartedAnime == true)
	{
		//アニメーションフラグをONにする
		isDuringAnimation_ = true;

		bool isExpandingX = true;
		if(scale_.x <= goalScale.x && isExpandingX == true)
		{
			scale_.x += animationSpeed;
			if(scale_.x >= goalScale.x)
			{
				isExpandingX = false;
			}
		}
		else if(scale_.x > goalScale.x && isExpandingX == true)
		{
			scale_.x -= animationSpeed;
			if(scale_.x <= goalScale.x)
			{
				isExpandingX = false;
			}
		}

		bool isExpandingY = true;
		if(scale_.y >= goalScale.y && isExpandingY == true)
		{
			scale_.y -= animationSpeed;
			if(scale_.x <= goalScale.x)
			{
				isExpandingY = false;
			}
		}
		else if(scale_.y < goalScale.y && isExpandingY == true)
		{
			scale_.y += animationSpeed;
			if(scale_.y >= goalScale.y)
			{
				isExpandingY = false;
			}
		}

		bool isExpandingZ = true;
		if(scale_.z <= goalScale.z && isExpandingZ == true)
		{
			scale_.z += animationSpeed;
			if(scale_.z >= goalScale.z)
			{
				isExpandingZ = false;
			}
		}
		else if(scale_.z > goalScale.z && isExpandingZ == true)
		{
			scale_.z -= animationSpeed;
			if(scale_.z <= goalScale.z)
			{
				isExpandingZ = false;
			}
		}

		if(isExpandingX == false && isExpandingY == false && isExpandingZ == false)
		{
			isDuringAnimation_ = false;
		}
	}
}


