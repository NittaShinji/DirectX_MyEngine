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

	transform_ = kPlayerInitPos_;
	rotation_ = { 0,0,0 };
	scale_ = { 1.0,1.0,1.0 };

	deadPos_ = transform_;

	//座標情報を設定
	SetTransform(transform_);
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
	isDead_ = false;

	isStartedJumpAnimation_ = false;
	isStartedLandAnime_ = false;
	isReturnedSizeAnime_ = false;
	isReadyToJump_ = false;
	isDuringAnimation_ = false;
	isPushOut_ = false;
	isRightAxcell_ = false;
	returnScaleSpeed_ = 0.15f;

	fallVec_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = { 0.0f,0.0f,0.0f };
	totalAxcell_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = Vector3Zero();

	attributeColor_ = Attribute::pink;
	colorFlag_ = true;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
}

void Player::Update(Camera* camera)
{
	gameCamera_ = camera;

	//瞬間のフラグをOFFにする
	if(isSecondJumpMoment_ == true)
	{
		isSecondJumpMoment_ = false;
	}

	//どのボタンが瞬間的に押されたか
	gamePad_->PushedButtonMoment();

	//ゲーム中プレイヤーが死んでいないとき
	if(isMoving_ == true && isfinish_ == false)
	{
		//合計加速度をリセット
		totalAxcell_ = { 0.0f,0.0f,0.0f };
		if(isDead_ == true)
		{
			stopSpeedEasing_.time--;
			if(stopSpeedEasing_.time >= 0)
			{
				totalAxcell_.z += PlayEaseOutQuint(stopSpeedEasing_);
			}
			else
			{
				isReset_ = true;
			}
		}
		else
		{
			totalAxcell_.z += kMoveAxcellZ_;
		}
		
		Accelerate();

		//落下処理
		if(!onGround_)
		{
			isGroundRotate_ = false;
			frameNum_ += freamIncreaseValue_;

			//if(frameNum_ >= JumpFrameNum_)
			//{
			//}
			////進行度を0～1の範囲に換算
			//float f = frameNum_ / JumpFrameNum_;

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

					isSecondJumpMoment_ = true;
					isTouchUnderObject_ = false;
					isJumpRotate_ = true;
					onGround_ = false;
					const float jumpVYFist = 0.4f;
					fallVec_ = { 0,jumpVYFist,0 };
					jumpCount--;
				}
				if(keys_->PushedKeyMoment(DIK_SPACE))
				{
					jumpSound_->PlaySoundWave(false);
					isSecondJumpMoment_ = true;
					isJumpRotate_ = true;
					isTouchUnderObject_ = false;
					onGround_ = false;
					const float jumpVYFist = 0.4f;
					fallVec_ = { 0,jumpVYFist,0 };
					jumpCount--;
				}
			}
		}
		//ジャンプ操作
		else if(keys_->PushedKeyMoment(DIK_SPACE) && jumpCount > 0)
		{
			jumpSound_->PlaySoundWave(false);
			isDentedAnime_ = true;
		}
		else if(gamePad_->GetButtonA() && jumpCount > 0)
		{
			jumpSound_->PlaySoundWave(false);
			isDentedAnime_ = true;
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

		//速度に加速度を加算
		if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::STOP)
		{
			
		}
		else
		{
			transform_.x += totalAxcell_.x;
			transform_.y += totalAxcell_.y;
			transform_.z += totalAxcell_.z * gameSpeed_->GetSpeedNum();
		}
		

		Object3d::SetTransform(transform_);
		UpdateWorldMatrix();
		collider_->Update();
	
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
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_PINK);
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_YELLOW);
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_BLACK);

		//交差による排斥分動かす
		if(onGround_ == false)
		{
			//transform_.x += callback.move.x;
			transform_.y += callback.move.y;
			transform_.z += callback.move.z;

			//押し出し時
			if(callback.move.z < 0)
			{
				isStoped_ = true;
				//isPushOut_ = true;
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
			isGroundRotate_ = true;
			GroundRotation();
			
			//スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			//接地を維持
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PINK, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
				transform_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_YELLOW, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
				transform_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_BLACK, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
				transform_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				Object3d::SetTransform(transform_);
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

			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PINK, &raycastHit, sphereCollider->GetRadius() * 2.0f))
			{
				//着地
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				isStartedLandAnime_ = true;
				transform_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = kMaxJumpNum;
				//行列の更新など
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_YELLOW, &raycastHit, sphereCollider->GetRadius() * 2.0f))
			{
				//着地
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				isStartedLandAnime_ = true;
				transform_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = kMaxJumpNum;
				//行列の更新など
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_BLACK, &raycastHit, sphereCollider->GetRadius() * 2.0f))
			{
				//着地
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				isStartedLandAnime_ = true;
				transform_.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
				jumpCount = kMaxJumpNum;
				//isResettingRotation_ = true;
				//行列の更新など
				Object3d::SetTransform(transform_);
			}

		}

		//落下処理
		if(transform_.y <= deadLine_)
		{
			//Sound::GetInstance()->PlaySoundWave("playerDead.wav", false);
			isDead_ = true;
			isLanded_ = true;
		}
	
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

		if(isStartChangeColorAnime_ == true && isStartedLandAnime_ == false && isReturnedSizeAnime_ == false && isDentedAnime_ == false && isExpandedAnime_ == false)
		{
			Animation(isStartChangeColorAnime_, kChangeColorScaleSpeed_, kChangeColorScale_);
			if(isDuringAnimation_ == false)
			{
				isReturnedSizeAnime_ = true;
				returnScaleSpeed_ = 0.075f;
				isStartChangeColorAnime_ = false;
			}
		}
		else
		{
			if(isStartedLandAnime_ == false && isReturnedSizeAnime_ == false && isDentedAnime_ == false && isExpandedAnime_ == false)
			{
				scale_ = { 1.0f,1.0f,1.0f };
			}
		}

		//着地時にへこむアニメーション
		if(isDead_ == false)
		{
			if(isStartedLandAnime_ == true)
			{
				if(keys_->PushedKeyMoment(DIK_SPACE))
				{
					if(jumpCount > 0)
					{
						isExpandedAnime_ = true;
						isDentedAnime_ = false;
						onGround_ = false;
						isLanded_ = false;
						const float jumpVYFist = 0.4f;
						fallVec_ = { 0,jumpVYFist,0 };
						jumpCount -= 1;
						isReadyToJump_ = false;
						isDuringAnimation_ = false;
						isStartedLandAnime_ = false;
					}
				}
				else
				{
					Animation(isStartedLandAnime_, kLandScaleSpeed_, kMaxLandMomentScale_);
					//アニメーションが終了し、拡大しない場合
					if(isDuringAnimation_ == false && isExpandedAnime_ == false)
					{
						//元のデフォルトサイズに戻す
						isReturnedSizeAnime_ = true;
						isStartedLandAnime_ = false;
					}
				}
			}
			//元のサイズに戻るアニメーション
			else if(isReturnedSizeAnime_ == true)
			{
				if(keys_->PushedKeyMoment(DIK_SPACE))
				{
					if(jumpCount > 0)
					{
						isExpandedAnime_ = true;
						isDentedAnime_ = false;
						onGround_ = false;
						isLanded_ = false;
						isRising_ = true;
						const float jumpVYFist = 0.4f;
						fallVec_ = { 0,jumpVYFist,0 };
						jumpCount -= 1;
						isReadyToJump_ = false;
						isDuringAnimation_ = false;
						isStartedLandAnime_ = false;
					}

				}
				else
				{
					Animation(isReturnedSizeAnime_, returnScaleSpeed_, kMoveScale_);
					if(isDuringAnimation_ == false)
					{
						isReturnedSizeAnime_ = false;
						returnScaleSpeed_ = 0.15f;
					}
				}
			}
			//ジャンプ時にへこむアニメーション
			else if(isDentedAnime_ == true)
			{
				Animation(isDentedAnime_, kDentSpeed_, kDentedScale_);
				if(isDuringAnimation_ == false)
				{
					if(jumpCount > 0)
					{
						isExpandedAnime_ = true;
						isDentedAnime_ = false;
						onGround_ = false;
						isLanded_ = false;
						isRising_ = true;
						const float jumpVYFist = 0.4f;
						fallVec_ = { 0,jumpVYFist,0 };
						jumpCount -= 1;
						isReadyToJump_ = false;
					}
				}
			}
			//ジャンプ後に伸びるアニメーション
			else if(isExpandedAnime_ == true)
			{
				Animation(isExpandedAnime_, kEpandSpeed_, kExpandScale_);
				if(isDuringAnimation_ == false)
				{
					isExpandedAnime_ = false;
					isReturnedSizeAnime_ = true;
					returnScaleSpeed_ = 0.15f;
				}
			}

			//二段ジャンプ時の回転処理
			if(isDuringAnimation_ == false)
			{
				JumpRotation();
			}
		}
	}
	
	//ResetRotation();

	if(isDead_ == false && isSetDeadPos_ == false)
	{
		deadPos_ = transform_;
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
		isDead_ = false;
	}
	//色が違う場合、死亡判定にする
	else if(info.object->GetAttributeColor() != attributeColor_ && isMoving_ == true)
	{
		isLanded_ = false;
		isDead_ = true;
		if(isSetDeadPos_ == false)
		{
			deadPos_ = transform_;
			isSetDeadPos_ = true;
		}
	}
	//プレイヤーと同じ色の場合
	else if(info.object->GetAttributeColor() == attributeColor_)
	{
		////球コライダーを取得
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(playerCollider_.get());

		Vector3 objectPosition = info.object->GetTransform();
		if(transform_.y + sphereCollider->GetRadius() <= objectPosition.y && isTouchUnderObject_ == false)
		{
			isTouchUnderObject_ = true;
			transform_ -= totalAxcell_;

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
	ray.start.y -= sphereCollider->GetRadius();
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PINK, &raycastHit, sphereCollider->GetRadius() * 3.0f))
	{
		//色を変える
		if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
		{
			if(attributeColor_ == yellow)
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
	else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_YELLOW, &raycastHit, sphereCollider->GetRadius() * 3.0f))
	{
		//色を変える
		if(gamePad_->GetButtonB() || keys_->PushedKeyMoment(DIK_RETURN))
		{
			if(attributeColor_ == pink)
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
}


void Player::EasingInitialize()
{
	axcellEasing_.time = 0;
}


void Player::Accelerate()
{
	if(isRightAxcell_ == true)
	{
		axcellEasing_.time++;
		if(axcellEasing_.time > 0 && axcellEasing_.time <= axcellEasing_.totalTime)
		{
			rightAxcellVec_.z = PlayEaseOutQuint(axcellEasing_);
		}
		else
		{
			rightAxcellVec_.z = 0;
			axcellEasing_.time = 0;
			isRightAxcell_ = false;
		}
	}

	totalAxcell_.z += rightAxcellVec_.z * gameSpeed_->GetSpeedNum();
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
	transform_ = kPlayerInitPos_;
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
	isRightAxcell_ = false;
	axcellEasing_.time = 0;
	rightAxcellVec_ = Vector3Zero();
	isJumpRotate_ = false;
	isGroundRotate_ = false;
	isResettingRotation_ = false;
	isSetDeadPos_ = false;
	rotateXTimer_ = kRotateXTime_;
	rotateYTimer_ = kRotateYTime_;

	attributeColor_ = Attribute::pink;
	SetColor(Vector3(1.0f, 0.4f, 0.7f));
	Object3d::SetTransform(transform_);
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

	ImGui::SliderFloat("PlayerPosY", &transform_.y, -100.0f, 50.0f);
	ImGui::SliderFloat("PlayerPosZ", &transform_.z, -100.0f, 1000.0f);
	ImGui::SliderFloat("PlayerScaleX", &scale_.x, 0.0f, 5.0f);
	ImGui::SliderFloat("PlayerScaleY", &scale_.y, 0.0f, 5.0f);
	ImGui::SliderFloat("PlayerScaleZ", &scale_.z, 0.0f, 5.0f);
	ImGui::SliderFloat("PlayerRotationX", &rotation_.x, 0.0f, 2.0f);
	ImGui::SliderFloat("PlayerRotationY", &rotation_.y, 0.0f, 2.0f);
	ImGui::SliderFloat("PlayerRotationZ", &rotation_.z, 0.0f, 2.0f);

	Object3d::SetScale(scale_);

	ImGui::End();
}

void Player::JumpRotation()
{
	//1回転する
	if(isJumpRotate_ == true)
	{
		if(scale_.x == 1.0f && scale_.y == 1.0f && scale_.z == 1.0f)
		{
			float angle = ToRadian(360.0f);
			rotation_.x -= PlayEaseIn(0.0f, angle, rotateXTimer_, kRotateXTime_);

			if(rotateXTimer_ >= 0)
			{
				rotateXTimer_--;
			}
			else
			{
				rotateXTimer_ = kRotateXTime_;
				isJumpRotate_ = false;
				rotation_.x = 0.0f;
			}
		}
	}

	Object3d::SetRotation(rotation_);
}

void Player::GroundRotation()
{
	if(isGroundRotate_ == false)
	{
		rotateYTimer_ = kRotateYTime_;
	}

	float rotateYVec = 0.10f;

	if(rotateYTimer_ >= 0)
	{
		rotateYTimer_--;
		
		float angle = ToRadian(360.0f);

		if(rotation_.y < angle)
		{
			rotation_.y -= rotateYVec;
		}
	}
	else
	{
		rotateYTimer_ = kRotateYTime_;
		isGroundRotate_ = false;
	}

	Object3d::SetRotation(rotation_);
}

void Player::Animation(bool isStartedAnime, float animationSpeed, Vector3 goalScale)
{
	if(isStartedAnime == true)
	{
		//アニメーション中フラグをONにする
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

void Player::ResetRotation()
{
	if(isResettingRotation_ == true)
	{
		bool isResetRotationX = true;
		if(isJumpRotate_ == false)
		{
			if(rotation_.x <= kResetRotation_.x && isResetRotationX == true)
			{
				rotation_.x += kRotaionSpeed_;
				if(rotation_.x >= kResetRotation_.x)
				{
					isResetRotationX = false;
				}
			}
			else if(rotation_.x > kResetRotation_.x && isResetRotationX == true)
			{
				rotation_.x -= kRotaionSpeed_;
				if(rotation_.x <= kResetRotation_.x)
				{
					isResetRotationX = false;
				}
			}
		}

		bool isResetRotationY = true;
		if(isGroundRotate_ == false)
		{	
			if(rotation_.y <= kResetRotation_.y && isResetRotationY == true)
			{
				rotation_.y += kRotaionSpeed_;
				if(rotation_.y >= kResetRotation_.y)
				{
					isResetRotationY = false;
				}
			}
			else if(rotation_.y > kResetRotation_.y && isResetRotationY == true)
			{
				rotation_.y -= kRotaionSpeed_;
				if(rotation_.y <= kResetRotation_.y)
				{
					isResetRotationY = false;
				}
			}
		}

		bool isResetRotationZ = true;
		if(rotation_.z <= kResetRotation_.z && isResetRotationZ == true)
		{
			rotation_.z += kRotaionSpeed_;
			if(rotation_.z >= kResetRotation_.z)
			{
				isResetRotationZ = false;
			}
		}
		else if(rotation_.z > kResetRotation_.z && isResetRotationZ == true)
		{
			rotation_.z -= kRotaionSpeed_;
			if(rotation_.z <= kResetRotation_.z)
			{
				isResetRotationZ = false;
			}
		}

		if(isResetRotationX == false && isResetRotationY == false && isResetRotationZ == false)
		{
			isResettingRotation_ = false;
		}
	}

}


