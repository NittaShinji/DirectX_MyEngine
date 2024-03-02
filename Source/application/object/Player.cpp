#include "Player.h"
#include "MathUtillity.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ObjectColor.h"
#include "Sound.h"
#include "ImGuiManager.h"

using namespace std;
using namespace NsEngine;

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
	//プレイヤー周りのサウンドを初期化
	SoundManager::GetInstance()->LoadSoundWave("jump.wav");
	SoundManager::GetInstance()->LoadSoundWave("doubleJump.wav");
	SoundManager::GetInstance()->LoadSoundWave("playerDead.wav");

	//サウンドのインスタンス生成
	jumpSound_ = std::make_unique<Sound>();
	doubleJumpSound_ = std::make_unique<Sound>();
	deadSound_ = std::make_unique<Sound>();
	//サウンドの初期化
	jumpSound_->Initialize("jump.wav");
	doubleJumpSound_->Initialize("doubleJump.wav");
	deadSound_->Initialize("playerDead.wav");

	//プレイヤーオブジェクトの初期化
	Object3d::Initialize();
	transform_ = kPlayerInitPos_;
	rotation_ = kDefaultRotate_;
	scale_ = kDefaultScale_;
	deadPos_ = transform_;
	SetTransform(transform_);
	SetRotation(rotation_);
	SetScale(scale_);

	//コライダーの追加
	const float radius = 1.0f;
	const Vector3 kDefaultColliderPos_ = { 0.0f,0.0f,0.0f };
	playerCollider_ = std::make_unique<SphereCollider>(Vector3(kDefaultColliderPos_), radius);

	//コライダーの登録
	SetCollider(playerCollider_.get());

	//属性を指定
	playerCollider_->SetAttribute(COLLISION_ATTR_ALLIES);

	//メンバ変数の初期化
	jumpCount_ = kMaxJumpNum_;
	isFlying_ = 0;
	isFinish_ = false;
	isMoving_ = false;
	isChangeColor = false;
	isJumpRotate_ = false;
	onGround_ = true;
	isLanded_ = false;
	isDead_ = false;
	isHit_ = false;
	isLongJump_ = false;
	isJumped_ = false;

	isStartedJumpAnimation_ = false;
	isStartedLandAnime_ = false;
	isReturnedSizeAnime_ = false;
	isDuringAnimation_ = false;
	isRightAxcell_ = false;
	isWithStageEdge_ = true;

	returnScaleSpeed_ = kDefaultReturnScaleSpeed_;

	fallVec_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = { 0.0f,0.0f,0.0f };
	totalAxcell_ = { 0.0f,0.0f,0.0f };
	rightAxcellVec_ = Vector3Zero();

	//プレイヤーの色の初期化
	attributeColor_ = Attribute::pink;
	colorFlag_ = true;
	Object3d::SetColorFlag(true);
	SetColor(kTitlePinkOBJColor);
}

void Player::Update(Camera* camera)
{
	gameCamera_ = camera;

	//着地フラグをオフに
	isLanded_ = false;

	//ゲームが始まっていないときに始める処理
	if(isDead_ == false && isFinish_ == false)
	{
		if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A))
		{
			isMoving_ = true;
		}

		if(keys_->PushedKeyMoment(DIK_SPACE))
		{
			isMoving_ = true;
		}
	}

	//瞬間のフラグをOFFにする
	if(isSecondJumpMoment_ == true)
	{
		isSecondJumpMoment_ = false;
	}

	//ゲーム中プレイヤーが死んでいないとき
	if(isMoving_ == true)
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

			if(isJumped_ == false)
			{
				const float slow = 10;
				//長押しの場合滑空するように
				if(keys_->HasPushedKey(DIK_SPACE) == true || gamePad_->HasPushedButton(XINPUT_GAMEPAD_A))
				{
					//下向き加速度　
					const float fallAcc = -0.015f;
					const float fallVYMin = -0.5f;
					
					if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::SLOW)
					{
						//加速
						fallVec_.y = max(fallVec_.y + (fallAcc / slow) , fallVYMin);
					}
					else
					{
						//加速
						fallVec_.y = max(fallVec_.y + fallAcc, fallVYMin);
					}
				}
				else
				{
					//下向き加速度　
					const float fallAcc = -0.021f;
					const float fallVYMin = -0.75f;

					if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::SLOW)
					{
						//加速
						fallVec_.y = max(fallVec_.y + (fallAcc / slow), fallVYMin);
					}
					else if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::STOP){}
					else
					{
						//加速
						fallVec_.y = max(fallVec_.y + fallAcc, fallVYMin);
					}
				}
			}

			//合計加速度に落下中の速度を加算
			totalAxcell_ += fallVec_;

			//二段ジャンプ時
			if(jumpCount_ > 0 && jumpCount_ < kMaxJumpNum_)
			{
				if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A))
				{
					jumpSound_->PlaySoundWave(false);

					isSecondJumpMoment_ = true;
					isTouchUnderObject_ = false;
					isJumpRotate_ = true;
					onGround_ = false;
					const float jumpVYFist = 0.4f;
					fallVec_ = { 0,jumpVYFist,0 };
					jumpCount_--;
				}
				if(keys_->PushedKeyMoment(DIK_SPACE))
				{
					jumpSound_->PlaySoundWave(false);
					isSecondJumpMoment_ = true;
					isTouchUnderObject_ = false;
					isJumpRotate_ = true;
					onGround_ = false;
					const float jumpVYFist = 0.4f;
					fallVec_ = { 0,jumpVYFist,0 };
					jumpCount_--;
				}
			}
		}
		//ジャンプ操作
		else if(keys_->PushedKeyMoment(DIK_SPACE) && jumpCount_ > 0)
		{
			jumpSound_->PlaySoundWave(false);
			isDentedAnime_ = true;
		}
		else if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A) && jumpCount_ > 0)
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

		//bool isKeepRunnig = false;
		//if(isFinish_ == true && isRightAxcell_ == false)
		//{
		//	isKeepRunnig = true;
		//}

		if(gameSpeed_->GetSpeedMode() == GameSpeed::SpeedMode::STOP) {}
		//その場で走り続ける
		/*else if(isKeepRunnig == true)
		{
			transform_= transform_;
		}*/
		else
		{
			transform_.x += totalAxcell_.x;
			transform_.y += totalAxcell_.y * gameSpeed_->GetSpeedNum();;
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
		if(attributeColor_ == pink)
		{
			CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_PINK);
		}
		else if(attributeColor_ == yellow)
		{
			CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_YELLOW);
		}
		else
		{
			CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_BLACK);
		}

		if(onGround_ == false)
		{
			transform_.y += callback.move.y;
			transform_.z += callback.move.z;

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
		const Vector3 downDir = { 0.0f,-1.0f,0.0f };
		ray.dir = downDir;
		RaycastHit raycastHit;

		const float colliderTwoTimesSize = sphereCollider->GetRadius() * 2.0f;

		//接地状態
		if(onGround_)
		{
			isGroundRotate_ = true;
			isJumped_ = true;
			if(isDuringAnimation_ == false)
			{
				jumpTotalValue_ = 0.0f;
			}

			GroundRotation();

			//スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			
			//接地を維持
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PINK, &raycastHit, colliderTwoTimesSize + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
				transform_.y -= (raycastHit.distance - colliderTwoTimesSize);
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_YELLOW, &raycastHit, colliderTwoTimesSize + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
				transform_.y -= (raycastHit.distance - colliderTwoTimesSize);
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_BLACK, &raycastHit, colliderTwoTimesSize + adsDistance))
			{
				onGround_ = true;
				isLanded_ = false;
				transform_.y -= (raycastHit.distance - colliderTwoTimesSize);
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
			
			if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PINK, &raycastHit, colliderTwoTimesSize))
			{
				//着地
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				if(isDuringAnimation_ == false)
				{
					isStartedLandAnime_ = true;
				}
				transform_.y -= (raycastHit.distance - colliderTwoTimesSize);
				jumpCount_ = kMaxJumpNum_;
				//行列の更新など
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_YELLOW, &raycastHit, colliderTwoTimesSize))
			{
				//着地
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				if(isDuringAnimation_ == false)
				{
					isStartedLandAnime_ = true;
				}
				transform_.y -= (raycastHit.distance - colliderTwoTimesSize);
				jumpCount_ = kMaxJumpNum_;
				//行列の更新など
				Object3d::SetTransform(transform_);
			}
			else if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_BLACK, &raycastHit, colliderTwoTimesSize))
			{
				//着地
				isTouchUnderObject_ = false;
				onGround_ = true;
				isLanded_ = true;
				if(isDuringAnimation_ == false)
				{
					isStartedLandAnime_ = true;
				}
				transform_.y -= (raycastHit.distance - colliderTwoTimesSize);
				jumpCount_ = kMaxJumpNum_;
				//行列の更新など
				Object3d::SetTransform(transform_);
			}

		}

		//落下処理
		if(transform_.y <= deadLine_)
		{
			isDead_ = true;
			isLanded_ = true;
		}

		//色変え処理
		if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_B) || keys_->PushedKeyMoment(DIK_RETURN))
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
				SetColor(kTitlePinkOBJColor);
			}
			else if(attributeColor_ == Attribute::yellow)
			{
				SetColor(kYellowOBJColor);
			}
			else
			{
				SetColor(kBlackOBJColor);
			}
		}

		Object3d::SetColorFlag(colorFlag_);

		if(isStartChangeColorAnime_ == true && isStartedLandAnime_ == false && isReturnedSizeAnime_ == false && isDentedAnime_ == false && isExpandedAnime_ == false)
		{
			Animation(isStartChangeColorAnime_, kChangeColorScaleSpeed_, kChangeColorScale_);
			if(isDuringAnimation_ == false)
			{
				isReturnedSizeAnime_ = true;
				returnScaleSpeed_ = kDefaultReturnScaleSpeed_;
				isStartChangeColorAnime_ = false;
			}
		}
		else
		{
			if(isStartedLandAnime_ == false && isReturnedSizeAnime_ == false && isDentedAnime_ == false && isExpandedAnime_ == false)
			{
				scale_ = kDefaultScale_;
			}
		}

		if(keys_->PushedKeyMoment(DIK_SPACE) == true || gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A))
		{
			isSmallJump_ = true;
		}
		else if(keys_->HasPushedKey(DIK_SPACE) == true || gamePad_->HasPushedButton(XINPUT_GAMEPAD_A))
		{
			if(isJumped_ == true)
			{
				if(jumpTotalValue_ < kInitJumpValue_ && jumpTotalValue_ < kMaxJumpValue_)
				{
					isSmallJump_ = true;
				}
				if(jumpTotalValue_ >= kInitJumpValue_ && jumpTotalValue_ < kMaxJumpValue_)
				{
					isSmallJump_ = false;
					isLongJump_ = true;
				}

				if(jumpTotalValue_ >= kMaxJumpValue_)
				{
					isJumped_ = false;
				}

			}
		}
		else if(keys_->ReleasedKeyMoment(DIK_SPACE) || gamePad_->ReleaseButtonMoment(XINPUT_GAMEPAD_A))
		{
			if(onGround_ == false && isSmallJump_ == false)
			{
				isJumped_ = false;
			}
		}
		else
		{
			if(onGround_ == false && isSmallJump_ == false)
			{
				isJumped_ = false;
			}
		}

		//着地時にへこむアニメーション
		if(isDead_ == false)
		{
			if(isStartedLandAnime_ == true)
			{
				//押した瞬間
				if(keys_->PushedKeyMoment(DIK_SPACE) || gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A))
				{
					if(jumpCount_ > 0)
					{
						isExpandedAnime_ = true;
						isDentedAnime_ = false;
						onGround_ = false;
						isLanded_ = false;

						jumpTotalValue_ = 0.0f;
						isSmallJump_ = true;
						isLongJump_ = false;
						LongJump();
						jumpCount_--;
						isDuringAnimation_ = false;
						isStartedLandAnime_ = false;
					}
				}
				else
				{
					Animation(isStartedLandAnime_, kLandScaleSpeed_, kMaxLandMomentScale_);
					//アニメーションが終了し、
					if(isDuringAnimation_ == false)
					{
						isStartedLandAnime_ = false;

						//拡大しない場合
						if(isExpandedAnime_ == false)
						{
							//元のデフォルトサイズに戻す
							isReturnedSizeAnime_ = true;
						}
					}
				}
			}
			//元のサイズに戻るアニメーション
			else if(isReturnedSizeAnime_ == true)
			{
				//押した瞬間
				if(keys_->PushedKeyMoment(DIK_SPACE) || gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_A))
				{
					if(jumpCount_ > 0)
					{
						isExpandedAnime_ = true;
						isDentedAnime_ = false;
						onGround_ = false;
						isLanded_ = false;
						LongJump();
						jumpCount_--;
						isDuringAnimation_ = false;
						isStartedLandAnime_ = false;
					}

				}
				else
				{
					LongJump();
					Animation(isReturnedSizeAnime_, returnScaleSpeed_, kMoveScale_);
					if(isDuringAnimation_ == false)
					{
						isReturnedSizeAnime_ = false;
						returnScaleSpeed_ = kAnimataionReturnScaleSpeed_;
					}
				}
			}
			//ジャンプ時にへこむアニメーション
			else if(isDentedAnime_ == true)
			{
				Animation(isDentedAnime_, kDentSpeed_, kDentedScale_);
				if(isDuringAnimation_ == false)
				{
					if(jumpCount_ > 0)
					{
						isExpandedAnime_ = true;
						isDentedAnime_ = false;
						onGround_ = false;
						isLanded_ = false;
						isRising_ = true;
						LongJump();
						jumpCount_--;
					}
				}
			}
			//ジャンプ後に伸びるアニメーション
			else if(isExpandedAnime_ == true)
			{
				LongJump();
				Animation(isExpandedAnime_, kEpandSpeed_, kExpandScale_);
				if(isDuringAnimation_ == false)
				{
					isExpandedAnime_ = false;
					isReturnedSizeAnime_ = true;
					returnScaleSpeed_ = kAnimataionReturnScaleSpeed_;
				}
			}

			//二段ジャンプ時の回転処理
			if(isDuringAnimation_ == false)
			{
				JumpRotation();
			}
		}
	}

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
		isRightAxcell_ = true;
		isFinish_ = true;
		isDead_ = false;
	}
	//色が違う場合、死亡判定にする
	else if(info.object->GetAttributeColor() != attributeColor_ && isMoving_ == true)
	{
		std::string modelName = info.object->GetModel()->GetName();

		if(modelName == "wall" && info.object->GetAttributeColor() != Goal)
		{
			if(isRightAxcell_ == true) {}
			else
			{
				isLanded_ = false;
				isDead_ = true;
				if(isSetDeadPos_ == false)
				{
					deadPos_ = transform_;
					isSetDeadPos_ = true;
				}
			}
		}
		else
		{
			isLanded_ = false;
			isDead_ = true;
			if(deadSound_->GetIsSounded() == false)
			{
				deadSound_->PlaySoundWave(false);
			}
			
			if(isSetDeadPos_ == false)
			{
				deadPos_ = transform_;
				isSetDeadPos_ = true;
			}
		}
	}
	//下からプレイヤーと同じ色のオブジェクトと当たった場合
	else if(info.object->GetAttributeColor() == attributeColor_)
	{
		if(onGround_ == false && isDuringAnimation_ == false && isExpandedAnime_ == false && isDentedAnime_ == false)
		{
			isTouchObject_ = true;
		}
		if(isExpandedAnime_ == true)
		{
			isTouchObject_ = true;
		}

		//球コライダーを取得
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
		if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_B) || keys_->PushedKeyMoment(DIK_RETURN))
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
		if(gamePad_->PushedButtonMoment(XINPUT_GAMEPAD_B) || keys_->PushedKeyMoment(DIK_RETURN))
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

void Player::Accelerate()
{
	if(isRightAxcell_ == true)
	{
		//加速後の猶予時間中でない場合スピードを足す
		if(isDuringAxcellExtensionTime_ == false)
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
				isDuringAxcellExtensionTime_ = true;
			}
		}
	}

	//加速後の猶予時間中
	if(isDuringAxcellExtensionTime_ == true)
	{
		//当たり判定用に
		axcellExtensionTime_++;
		if(axcellExtensionTime_ >= maxExtensionTime_)
		{
			isRightAxcell_ = false;
			axcellExtensionTime_ = 0;
			isDuringAxcellExtensionTime_ = false;
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
	jumpSound_->Initialize("jump.wav");
	doubleJumpSound_->Initialize("doubleJump.wav");
	deadSound_->Initialize("playerDead.wav");

	transform_ = kPlayerInitPos_;
	rotation_ = kDefaultRotate_;
	scale_ = kDefaultScale_;
	jumpCount_ = kMaxJumpNum_;
	jumpTotalValue_ = 0.0f;
	onGround_ = true;
	isJumpRotate_ = false;
	isFlying_ = 0;
	isFinish_ = false;
	isMoving_ = false;
	isDead_ = false;
	isLanded_ = false;
	isChangeColor = false;
	isStartedLandAnime_ = false;
	isReturnedSizeAnime_ = false;
	isStartedJumpAnimation_ = false;
	isDentedAnime_ = false;
	isRightAxcell_ = false;
	isWithStageEdge_ = true;
	axcellEasing_.time = 0;
	rightAxcellVec_ = Vector3Zero();
	isJumpRotate_ = false;
	isGroundRotate_ = false;
	isResettingRotation_ = false;
	isSetDeadPos_ = false;
	rotateXTimer_ = kRotateXTime_;
	rotateYTimer_ = kRotateYTime_;

	attributeColor_ = Attribute::pink;
	SetColor(kTitlePinkOBJColor);
	Object3d::SetTransform(transform_);
	Object3d::SetRotation(rotation_);
	Object3d::SetScale(scale_);
	Object3d::Update(camera);
}

void Player::ImGuiUpdate()
{
	//スプライトの編集ウインドウの表示

	ImGui::Begin("Player");

	const Vector2 kImGuiPos = { 0.0f,0.0f };
	const Vector2 kImGuiSize = { 300.0f,200.0f };

	ImGui::SetWindowPos(ImVec2(kImGuiPos.x, kImGuiPos.y));
	ImGui::SetWindowSize(ImVec2(kImGuiSize.x, kImGuiSize.y));

	const Vector2 kImGuiPosRate = { -100.0f,1000.0f };
	const Vector2 kImGuiSizeRate = { 0.0f,5.0f };
	const Vector2 kImGuiRotateRate = { 0.0f,2.0f };

	ImGui::SliderFloat("PlayerPosY", &transform_.y, kImGuiPosRate.x, kImGuiPosRate.x);
	ImGui::SliderFloat("PlayerPosZ", &transform_.z, kImGuiPosRate.x, kImGuiPosRate.x);
	ImGui::SliderFloat("PlayerScaleX", &scale_.x, kImGuiSizeRate.x, kImGuiSizeRate.y);
	ImGui::SliderFloat("PlayerScaleY", &scale_.y, kImGuiSizeRate.x, kImGuiSizeRate.y);
	ImGui::SliderFloat("PlayerScaleZ", &scale_.z, kImGuiSizeRate.x, kImGuiSizeRate.y);
	ImGui::SliderFloat("PlayerRotationX", &rotation_.x, kImGuiRotateRate.x, kImGuiRotateRate.y);
	ImGui::SliderFloat("PlayerRotationY", &rotation_.y, kImGuiRotateRate.x, kImGuiRotateRate.y);
	ImGui::SliderFloat("PlayerRotationZ", &rotation_.z, kImGuiRotateRate.x, kImGuiRotateRate.y);

	Object3d::SetScale(scale_);

	ImGui::End();
}

void Player::JumpRotation()
{
	//1回転する
	if(isJumpRotate_ == true)
	{
		if(scale_.x == kDefaultScale_.x && scale_.y == kDefaultScale_.y && scale_.z == kDefaultScale_.z)
		{
			float angle = ToRadian(kOneCircleRotate_);
			rotation_.x -= PlayEaseInCubic(0.0f, angle, rotateXTimer_, kRotateXTime_);

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

	//回転角をセット
	Object3d::SetRotation(rotation_);
}

void Player::GroundRotation()
{
	if(isGroundRotate_ == false)
	{
		rotateYTimer_ = kRotateYTime_;
	}

	//1回転させる
	if(rotateYTimer_ >= 0)
	{
		rotateYTimer_--;

		float angle = ToRadian(kOneCircleRotate_);

		if(rotation_.y < angle)
		{
			const float rotateYVec = 0.10f;
			rotation_.y -= rotateYVec;
		}
	}
	else
	{
		rotateYTimer_ = kRotateYTime_;
		isGroundRotate_ = false;
	}

	//回転角をセット
	Object3d::SetRotation(rotation_);
}

void Player::Animation(const bool isStartedAnime, const float animationSpeed, const Vector3& goalScale)
{
	if(isStartedAnime == true)
	{
		//アニメーション中フラグをONにする
		isDuringAnimation_ = true;
		ResetVector3Value(scale_, goalScale, animationSpeed, isDuringAnimation_);
	}
}

void Player::ResetRotation()
{
	//ジャンプなどによって、回転が元に戻っていなければ元の角度に戻す
	if(isResettingRotation_ == true)
	{
		ResetVector3Value(rotation_, kResetRotation_, kRotaionSpeed_, isResettingRotation_);
	}
}

void Player::LongJump()
{
	//押した時間によって飛距離が変わるように
	if(isSmallJump_ == true)
	{
		if(isJumped_ == true)
		{
			if(jumpTotalValue_ < kMaxJumpValue_)
			{
				//加速
				jumpTotalValue_ = min(jumpTotalValue_ + kInitJumpValue_, kMaxJumpValue_);
				fallVec_.y = jumpTotalValue_;
			}
			else
			{
				isJumped_ = false;
			}
		}
		isSmallJump_ = false;
	}
	else if(keys_->HasPushedKey(DIK_SPACE) || gamePad_->HasPushedButton(XINPUT_GAMEPAD_A))
	{
		if(isLongJump_ == true)
		{
			if(isJumped_ == true)
			{
				const float jumpIncrease = 0.015f;

				if(jumpTotalValue_ < kMaxJumpValue_)
				{
					//加速
					jumpTotalValue_ = min(jumpTotalValue_ + jumpIncrease, kMaxJumpValue_);
					fallVec_.y = jumpTotalValue_;
				}
				else
				{
					jumpTotalValue_ = kMaxJumpValue_;
					isJumped_ = false;
					isLongJump_ = false;
				}
			}
		}
	}
}

bool Player::ResetValue(float& value, const float defaultValue, const float changeValue)
{
	bool result = false;

	if(value < defaultValue)
	{
		value += changeValue;
		if(value >= defaultValue)
		{
			result = true;
		}
	}
	else if(value > defaultValue)
	{
		value -= changeValue;
		if(value <= defaultValue)
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}

	return result;
}

void Player::ResetVector3Value(Vector3& value, const Vector3 defaultValue, const float changeValue, bool& isStartToReset)
{
	bool isResetedX= false;
	isResetedX = ResetValue(value.x, defaultValue.x, changeValue);

	bool isResetedY = false;
	isResetedY = ResetValue(value.y, defaultValue.y, changeValue);

	bool isResetedZ = false;
	isResetedZ = ResetValue(value.z, defaultValue.z, changeValue);

	//もし全て戻っていたらフラグをオフにする
	if(isResetedX == true && isResetedY == true && isResetedZ == true)
	{
		isStartToReset = false;
	}
}