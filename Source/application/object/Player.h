#pragma once
#include"Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "ObjectAttribute.h"
#include "SoundManager.h"
#include "Easing.h"
#include "GameSpeed.h"

class Player : public Object3d
{
public:

	static std::unique_ptr<Player> Create(const std::string& path);

public:

	void Initialize() override;

	void Update(Camera* camera) override;

	void OnCollision(const CollisionInfo& info) override;

	//加速する
	void AccelerateChangeColor();

	void Accelerate();

	void Draw();

	void Reset(Camera* camera);

	void ImGuiUpdate();

	void JumpRotation();

	void GroundRotation();

	void ColorChangeAnimation();

	void EasingInitialize();

	void Animation(bool isStartedAnime,float animationSpeed, Vector3 goalScale);

	void ResetRotation();

private:

	std::unique_ptr<Object3d> object_;

	//初期位置
	const Vector3 kPlayerInitPos_ = { 0.0f,2.0f,2.0f };

	Vector3 move = { 0,0,0 };

	//ジャンプカウント
	int32_t jumpCount;

	//jumpheightの最大値
	const float kMaxJump = 0.1f;

	//最大ジャンプ回数
	int32_t kMaxJumpNum = 2;;

	//空中にいるかどうか
	bool isFlying_;

	//重力
	float gravitySpeed = 0.0f;

	//動いているか
	bool isMoving_;

	//ゴールしたかどうか
	bool isfinish_;

	//プレイヤーのコライダー
	std::unique_ptr<SphereCollider> playerCollider_ = nullptr;

	//プレイヤー用のコントローラー
	//std::unique_ptr<GamePad> player_ = nullptr;
	GamePad* gamePad_ = nullptr;

	//接地フラグ
	bool onGround_ = false;

	//死亡フラグ
	bool isDead_ = false;

	//落下ベクトル
	Vector3 fallVec_;

	//色変えフラグ
	bool isChangeColor = false;

	const int32_t pushTime = 1;

	int32_t colorTimer_ = pushTime;

	bool pushMoment_ = false;

	Attribute attributeColor_;

	//ジャンプ時の加速度
	float jumpAcc_ = 0.0f;

	//合計加速度
	Vector3 totalAxcell_;

	//通常加速度
	const float kMoveAxcellZ_ = 0.78f;
	//死んだ際の加速度
	const float kDeadMoveAxcellZ_ = kMoveAxcellZ_ / 2;

	//右向き加速ベクトル
	Vector3 rightAxcellVec_;

	//加速フラグ
	bool isRightAxcell_;
	//加速時間
	const int32_t kAxcellTime_ = 30;
	//加速計測時間
	int32_t axcellTimer_ = kAxcellTime_;

	//プレイヤー死亡ライン
	const float deadLine_ = -5.0f;

	//プレイヤーが死亡した場所
	Vector3 deadPos_;

	//1周回る時間
	const float kRotateXTime_ = 60.0f;
	const float kRotateYTime_ = 60.0f;

	//回転時間
	float rotateXTimer_ = kRotateXTime_;
	float rotateYTimer_ = kRotateYTime_;

	//回転フラグ
	bool isJumpRotate_ = false;

	//スケールアニメーション時間
	const float kScaleTime_ = 60.0f;
	//スケールタイマー
	float scaleTime_ = kScaleTime_;

	//スケールアニメーションフラグ
	bool isStartChangeColorAnime_ = false;

	//着地フラグ
	bool isLanded_;

	//ゲームカメラ
	Camera* gameCamera_;

	//下からオブジェクトに触れたかどうか
	bool isTouchUnderObject_;

	//ぶつかってストップしているか
	bool isStoped_;

	//押し出されたかどうか
	bool isPushOut_;


	bool isStartedJumpAnimation_;
	int32_t kJumpAnimationTime_ = 10;
	int32_t jumpAnimationTimer_ = kJumpAnimationTime_;

	const Vector3 kMaxJumpMomentScale_ = { 1.0f,0.8f,1.2f };
	const Vector3 kMaxLandMomentScale_ = { 2.0f,0.5f,2.0f };
	const Vector3 kMoveScale_ = {1.0f,1.0f,1.0f};
	const Vector3 kDentedScale_ = {1.8f,0.4f,1.8f};
	const Vector3 kExpandScale_ = {0.4f,1.5f,0.4f};
	const Vector3 kChangeColorScale_ = {1.3f,1.3f,1.3f};
	const Vector3 kResetRotation_ = { 0.0f,0.0f,0.0f };
	
	const float kLandScaleSpeed_ = 0.175f;
	//const float kReturnScaleSpeed_ = 0.15f;
	const float kDentSpeed_ = 0.175f;
	const float kEpandSpeed_ = 0.15f;
	const float kChangeColorScaleSpeed_ = 0.04f;
	const float kRotaionSpeed_ = 0.1f;

	float returnScaleSpeed_ = 0.10f;

	EasingInfo jumpEasing_ = { 0.0f, 0.0f, 0.0f, 60.0f };
	EasingInfo LandEasing_ = { 0.0f, 0.0f, 0.0f, 45.0f };
	EasingInfo axcellEasing_ = { 0.5f, 0.0f, 0.0f, 25.0f };
	EasingInfo stopSpeedEasing_ = { kMoveAxcellZ_,-kMoveAxcellZ_,0.0f,60.0f };
	
	bool isStartedLandAnime_ = false;
	bool isReturnedSizeAnime_ = false;
	bool isJumpMomentAnime_ = false;
	bool isDentedAnime_ = false;
	bool isExpandedAnime_ = false;

	//ジャンプの準備ができているかどうか
	bool isReadyToJump_;

	bool isScaleChanged_ = false;
	bool isDuringAnimation_ = false;
	bool isResettingRotation_ = false;

	bool isGroundRotate_ = false;
	bool isReset_ = false;
	bool isSetDeadPos_ = false;

	//フレーム増加量
	const float freamIncreaseValue_ = 1.0f;
	float frameNum_;
	float JumpFrameNum_ = 60.0f;

	bool isRising_ = false;
	bool isSecondJumpMoment_ = false;

	GameSpeed* gameSpeed_ = nullptr;

	std::unique_ptr<Sound> jumpSound_ = nullptr;
	std::unique_ptr<Sound> doubleJumpSound_ = nullptr;
public:

	bool GetIsMoving() { return isMoving_; }
	bool GetIsFinish() { return isfinish_; }
	bool GetIsDead() { return isDead_; }
	bool GetIsLanded() { return isLanded_; }
	bool GetOnGround() { return onGround_; }
	Attribute GetAttributeColor() { return attributeColor_; }
	Vector3 GetPlayerInitPos() { return kPlayerInitPos_; }
	Vector3 GetTotalAxcell() { return totalAxcell_; }
	bool GetRightAxcell() { return isRightAxcell_; }
	bool GetIsStoped() { return isStoped_; }
	Vector3 GetInitPos() { return kPlayerInitPos_; }
	float GetNormalAxcellZ() { return kMoveAxcellZ_; }
	Vector3 GetDeadPos() { return deadPos_; }
	bool GetIsSecondJumpMoment() { return isSecondJumpMoment_; }

	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }
	void SetGamePad(GamePad* gamePad) { gamePad_ = gamePad; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }
};

