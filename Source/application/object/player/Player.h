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
#include "QueryCallback.h"

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
	void Reset();

	//Imgui更新
	void ImGuiUpdate();

	//ジャンプ中に回転する
	void JumpRotation();

	//地面についているときに回転する
	void GroundRotation();

	//アニメーション
	void Animation(const bool isStartedAnime, const float animationSpeed, const Vector3& goalScale);

	//ロングジャンプ
	void LongJump();

	//ジャンプアニメーションした際の共通処理をまとめたもの
	void UpdatePlayerAnimation();

	//数値を規定の数値に戻す関数
	bool ResetValue(float& value, const float defaultValue, const float changeValue);
	void ResetVector3Value(Vector3& value, const Vector3 defaultValue, const float changeValue, bool& isStartToReset);

	//死んだときの処理
	void UpdateWhenDead();

	//加速できる色かチェックする
	void CheckPlayerCanAccelColor();

	//加速できる状態で加速できることを示す白色に変更する
	void ChangeColorWhiteforAccel();

private: //定数

	//最大ジャンプ回数
	const int32_t kMaxJumpNum_ = 2;

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
	const float deadLine_ = -2.1f;
	//1周回る時間
	const float kRotateXTime_ = 60.0f;
	const float kRotateYTime_ = 60.0f;

	//1週分の回転角
	const float kOneCircleRotate_ = 360.0f;

	//初期カウンター数
	const int32_t kInitCount_ = 0;

	//通常スケール値
	const Vector3 kDefaultScale_ = { 1.0f,1.0f,1.0f };
	//通常回転値
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
	const float maxExtensionTime_ = 10.0f;

	//元のスケールに戻る際の通常スピード
	const float kDefaultReturnScaleSpeed_ = 0.075f;
	//元のスケールに戻る際のアニメーション中のスピード
	const float kAnimataionReturnScaleSpeed_ = 0.15f;

	//プレイヤーの衝突範囲(加速判定などもあるので球コライダーの半径より大きめに)
	const float kCollisionArea_ = 30.0f;

	//加速距離
	const float kAccelDistance_ = 4.9f;
	
private:

	//オブジェクト
	std::unique_ptr<Object3d> object_;

	//ジャンプカウント
	int32_t jumpCount_;
	
	//ジャンプ量
	float jumpTotalValue_;
	
	bool isJumped_ = false;
	bool isLongJump_ = false;
	bool isSmallJump_ = false;

	//動いているか
	bool isMoving_;

	//ゴールしたかどうか
	bool isFinish_;

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

	//ジャンプ入力を受け付けるか
	bool canInputJump_ = true;
	//色変え入力を受け付けるか
	bool canInputColor_ = true;

	//着地フラグ
	bool isLanded_;

	//ゲームカメラ
	Camera* gameCamera_;

	//下からオブジェクトに触れたかどうか
	bool isTouchUnderObject_;

	//オブジェクトに触れたかどうか
	bool isTouchObject_;

	EasingInfo jumpEasing_ = { 0.0f, 0.0f, 0.0f, 60.0f };
	EasingInfo LandEasing_ = { 0.0f, 0.0f, 0.0f, 45.0f };
	EasingInfo axcellEasing_ = { 0.5f, 0.0f, 0.0f, 25.0f };
	EasingInfo stopSpeedEasing_ = { kMoveAxcellZ_,-kMoveAxcellZ_,0.0f,60.0f };
	
	//アニメーションフラグ
	bool isStartedLandAnime_ = false;
	bool isReturnedSizeAnime_ = false;
	bool isJumpMomentAnime_ = false;
	bool isDentedAnime_ = false;
	bool isExpandedAnime_ = false;

	//ジャンプの準備ができているかどうか
	bool isReadyToJump_;
	//アニメーション中かどうか
	bool isDuringAnimation_ = false;
	//接地中に開店するかどうか
	bool isGroundRotate_ = false;
	//死亡時に死んだ場所を保存したかどうか
	bool isSetDeadPos_ = false;
	//二回目のジャンプをした瞬間かどうか
	bool isSecondJumpMoment_ = false;

	//ステージの端にいるかどうか
	bool isWithStageEdge_ = true;

	//加速判定猶予用の時間
	bool isDuringAxcellExtensionTime_;
	float axcellExtensionTime_ = 0.0f;

	//加速できるかどうか
	bool canAccel_ = false;
	//加速できる色か
	bool isAccelColor_;

	//プレイヤーと床までの距離
	float distanceFloor = 0.0f;

	GameSpeed* gameSpeed_ = nullptr;

	//サウンド
	std::unique_ptr<Sound> jumpSound_ = nullptr;
	std::unique_ptr<Sound> doubleSound_ = nullptr;
	std::unique_ptr<Sound> deadSound_ = nullptr;

public: //アクセッサ

	//動いているどうかを取得する
	bool GetIsMoving() const { return isMoving_; }
	//ゴールしたかどうかを取得する
	bool GetIsFinish() const { return isFinish_; }
	//死んでいるかどうかを取得する
	bool GetIsDead() const { return isDead_; }
	//着地したかどうかを取得する
	bool GetIsLanded() const { return isLanded_; }
	//地面にいるかどうかを取得する
	bool GetOnGround() const { return onGround_; }
	//プレイヤーの色を取得する
	Attribute GetAttributeColor() const { return attributeColor_; }
	//加速しているかどうかを取得する
	bool GetRightAxcell() const { return isRightAxcell_; }
	//プレイヤーの初期座標を取得する
	Vector3 GetInitPos() const { return kPlayerInitPos_; }
	//死んだ位置を取得する
	Vector3 GetDeadPos() const { return deadPos_; }
	//二回目のジャンプ時かどうかを取得する
	bool GetIsSecondJumpMoment() const { return isSecondJumpMoment_; }
	//下からオブジェクトに触れたかどうかを取得する
	bool GetIsTouchObject() const { return isTouchObject_; }
	//プレイヤーの衝突可能範囲
	float GetCollisionArea() const { return kCollisionArea_; }
	//ジャンプカウントを取得
	int32_t GetJumpCount() const { return jumpCount_; }
	//最大ジャンプ数を取得
	int32_t GetKMaxJumpNum() const { return kMaxJumpNum_; }
	//ステージの端にいるかどうか
	bool GetIsWithStageEdge() const { return isWithStageEdge_; }
	//合計加速度を取得
	Vector3 GetTotalAxcell() const { return totalAxcell_; }
	//ジャンプ入力を受け付けるかどうかを取得
	bool GetCanInputJump() const { return canInputJump_; }
	//色変え入力を受け付けるかどうかを取得
	bool GetCanInputColor() const { return canInputColor_; }
	//加速できるかどうかを取得
	bool GetCanAccel() const { return canAccel_; }
	//加速できるかどうかを取得
	bool GetIsAccelColor() const { return isAccelColor_; }
	//地面までの距離を取得
	float GetDistanceFloor() const { return distanceFloor; }
	//加速までの距離を取得
	float GetAccelDistance() const { return kAccelDistance_; }

	//動いているかどうかをセットする
	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }
	//ゲームパッドをセット
	void SetGamePad(GamePad* gamePad) { gamePad_ = gamePad; }
	//ゲームスピードをセットする
	void SetGameSpeed(GameSpeed* gameSpeed) { gameSpeed_ = gameSpeed; }
	//ゴールしたどうかをセットする
	void SetIsFinish(bool isFinish) { isFinish_ = isFinish; }
	//下からオブジェクトに触れたかどうかをセットする
	void SetIsTouchObject(bool isTouchObject) { isTouchObject_ = isTouchObject; }
	//加速しているかどうかをセットする
	void SetIsRightAxcell(bool isRightAxcell) { isRightAxcell_ = isRightAxcell; }
	//ステージの端にいるかどうか
	void SetIsWithStageEdge(bool isWithStageEdge) { isWithStageEdge_ = isWithStageEdge; }
	//プレイヤーの色をセット
	void SetAttributeColor(Attribute AttributeColor) { attributeColor_ = AttributeColor; }
	//ジャンプ入力を受け付けるかどうかをセットする
	void SetCanInputJump(bool canInputJump) { canInputJump_ = canInputJump; }
	//色変え入力を受け付けるかどうかをセットする
	void SetCanInputColor(bool canInputColor) { canInputColor_ = canInputColor; }
	//死んだかどうかをセットする
	void SetIsDead(bool isDead) { isDead_ = isDead; }
};

