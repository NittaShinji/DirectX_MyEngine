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

/// <summary>
/// プレイヤー
/// </summary>
class Player : public Object3d
{
public:

	//インスタンスを生成
	static std::unique_ptr<Player> Create(const std::string& path);

public:

	//初期化
	void Initialize() override;

	//更新
	void Update(Camera* camera) override;

	//衝突時に呼び出される関数
	void OnCollision(const CollisionInfo& info) override;

	//タイミングよく色変えした時に加速する関数
	void AccelerateChangeColor();

	//加速する関数
	void Accelerate();

	//描画
	void Draw();

	//リセット
	void Reset(Camera* camera);

	//Imgui更新
	void ImGuiUpdate();

	//ジャンプ中に回転する
	void JumpRotation();

	//地面についているときに回転する
	void GroundRotation();

	//アニメーション
	void Animation(bool isStartedAnime,float animationSpeed, Vector3 goalScale);

	//回転をリセットする
	void ResetRotation();

	//ロングジャンプ
	void LongJump();

private: //定数

	//最大ジャンプ回数
	const int32_t kMaxJumpNum = 2;

	//初期位置
	const Vector3 kPlayerInitPos_ = { 0.0f,2.0f,2.0f };
	//最大ジャンプ量
	const float kMaxJumpValue_ = 0.38f;
	const float kInitJumpValue_ = 0.27f;
	//通常加速度
	const float kMoveAxcellZ_ = 0.78f;
	//死んだ際の加速度
	const float kDeadMoveAxcellZ_ = kMoveAxcellZ_ / 2;
	//加速時間
	const int32_t kAxcellTime_ = 30;
	//プレイヤー死亡ライン
	const float deadLine_ = -5.0f;
	//1周回る時間
	const float kRotateXTime_ = 60.0f;
	const float kRotateYTime_ = 60.0f;

	//1週分の回転角
	const float kOneCircleRotate = 360.0f;

	//初期カウンター数
	const int32_t kInitCount_ = 0;

	const Vector3 kDefaultScale_ = { 1.0f,1.0f,1.0f };
	const Vector3 kDefaultRotate_ = { 0.0f,0.0f,0.0f };


	//アニメーション用スケール定数
	const Vector3 kMaxJumpMomentScale_ = { 1.0f,0.8f,1.2f };
	const Vector3 kMaxLandMomentScale_ = { 2.0f,0.5f,2.0f };
	const Vector3 kMoveScale_ = { 1.0f,1.0f,1.0f };
	const Vector3 kDentedScale_ = { 1.8f,0.4f,1.8f };
	const Vector3 kExpandScale_ = { 0.4f,1.5f,0.4f };
	const Vector3 kChangeColorScale_ = { 1.3f,1.3f,1.3f };
	const Vector3 kResetRotation_ = { 0.0f,0.0f,0.0f };

	//アニメーション変化スピード
	const float kLandScaleSpeed_ = 0.175f;
	const float kDentSpeed_ = 0.175f;
	const float kEpandSpeed_ = 0.15f;
	const float kChangeColorScaleSpeed_ = 0.04f;
	const float kRotaionSpeed_ = 0.1f;
	const int32_t kJumpAnimationTime_ = 10;
	//フレーム増加量
	const float freamIncreaseValue_ = 1.0f;
	//加速判定最大時間
	const float maxExtensionTime = 10.0f;

	const float kDefaultReturnScaleSpeed_ = 0.075f;
	const float kAnimataionReturnScaleSpeed_ = 0.15f;
	
private:

	//オブジェクト
	std::unique_ptr<Object3d> object_;

	Vector3 move = { 0,0,0 };

	//ジャンプカウント
	int32_t jumpCount;

	
	//空中にいるかどうか
	bool isFlying_;

	//ジャンプ量
	float jumpTotalValue_;

	
	bool isJumped_ = false;
	bool isLongJump_ = false;
	bool isSmallJump_ = false;

	//動いているか
	bool isMoving_;

	//ゴールしたかどうか
	bool isfinish_;

	//プレイヤーのコライダー
	std::unique_ptr<SphereCollider> playerCollider_ = nullptr;

	//プレイヤー用のコントローラー
	GamePad* gamePad_ = nullptr;

	//接地フラグ
	bool onGround_ = false;

	//死亡フラグ
	bool isDead_ = false;

	//落下ベクトル
	Vector3 fallVec_;

	//色変えフラグ
	bool isChangeColor = false;

	//プレイヤーの色
	Attribute attributeColor_;

	//合計加速度
	Vector3 totalAxcell_;

	//右向き加速ベクトル
	Vector3 rightAxcellVec_;

	//加速フラグ
	bool isRightAxcell_;
	
	//加速計測時間
	int32_t axcellTimer_ = kAxcellTime_;

	//プレイヤーが死亡した場所
	Vector3 deadPos_;

	//回転時間
	float rotateXTimer_ = kRotateXTime_;
	float rotateYTimer_ = kRotateYTime_;

	//元のスケールに戻るスピード
	float returnScaleSpeed_;

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

	//オブジェクトに触れたかどうか
	bool isTouchObject_;

	//ぶつかってストップしているか
	bool isStoped_;

	bool isStartedJumpAnimation_;
	
	int32_t jumpAnimationTimer_ = kJumpAnimationTime_;
	
	
	EasingInfo jumpEasing_ = { 0.0f, 0.0f, 0.0f, 60.0f };
	EasingInfo LandEasing_ = { 0.0f, 0.0f, 0.0f, 45.0f };
	EasingInfo axcellEasing_ = { 0.5f, 0.0f, 0.0f, 20.0f };
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

	float frameNum_;

	bool isRising_ = false;
	bool isSecondJumpMoment_ = false;
	bool isHit_ = false;

	//加速判定猶予用の時間
	bool isDuringAxcellExtensionTime_;
	float axcellExtensionTime_ = 0.0f;
	
	GameSpeed* gameSpeed_ = nullptr;

	std::unique_ptr<Sound> jumpSound_ = nullptr;
	std::unique_ptr<Sound> doubleJumpSound_ = nullptr;

public: //アクセッサ

	//動いているどうかを取得する
	bool GetIsMoving() { return isMoving_; }
	//ゴールしたかどうかを取得する
	bool GetIsFinish() { return isfinish_; }
	//死んでいるかどうかを取得する
	bool GetIsDead() { return isDead_; }
	//着地したかどうかを取得する
	bool GetIsLanded() { return isLanded_; }
	//地面にいるかどうかを取得する
	bool GetOnGround() { return onGround_; }
	//プレイヤーの色を取得する
	Attribute GetAttributeColor() { return attributeColor_; }
	//加速しているかどうかを取得する
	bool GetRightAxcell() { return isRightAxcell_; }
	//プレイヤーの初期座標を取得する
	Vector3 GetInitPos() { return kPlayerInitPos_; }
	//死んだ位置を取得する
	Vector3 GetDeadPos() { return deadPos_; }
	//二回目のジャンプ時かどうかを取得する
	bool GetIsSecondJumpMoment() { return isSecondJumpMoment_; }
	//下からオブジェクトに触れたかどうかを取得する
	bool GetIsTouchObject() { return isTouchObject_; }

	//動いているかどうかをセットする
	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }
	//ゲームパッドをセット
	void SetGamePad(GamePad* gamePad) { gamePad_ = gamePad; }
	//ゲームスピードをセットする
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }
	//ゴールしたどうかをセットする
	void SetIsFinish(bool isFinish) { isfinish_ = isFinish; }
	//下からオブジェクトに触れたかどうかをセットする
	void SetIsTouchObject(bool isTouchObject) { isTouchObject_ = isTouchObject; }
	//加速しているかどうかをセットする
	void SetIsRightAxcell(bool isRightAxcell) { isRightAxcell_ = isRightAxcell; }
};

