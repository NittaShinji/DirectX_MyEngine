#pragma once
#include"Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "GamePad.h"
#include "SphereCollider.h"
#include "ObjectAttribute.h"

class Player : public Object3d
{
public:

	static std::unique_ptr<Player> Create(const std::string& path);

public:

	void Initialize() override;

	void Update(Camera* camera) override;

	void OnCollision(const CollisionInfo& info) override;

	//加速する
	void AccelerateChangeColor(Camera* camera);

	void Accelerate();

	void SetNextState();

	void Draw();

	void finish();

	void Reset();

	void ImGuiUpdate();

private:

	std::unique_ptr<Object3d> object_;

	//初期位置
	Vector3 playerInitPos_;
	//位置
	Vector3 position_;
	Vector3 rotation_ = { 0,0,0 };
	Vector3 scale_ = { 1,1,1 };

	Vector3 move = { 0,0,0 };

	//ジャンプカウント
	int32_t jumpCount;

	// 等速ジャンプの1フレームごとのジャンプの上がり幅
	float jumpHeight;

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

	//半径
	float radius = 0.6f;

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


	bool isChangeColor = false;

	const int32_t pushTime = 1;

	int32_t colorTimer_ = pushTime;

	bool pushMoment_ = false;

	Attribute attribute_;

	//ジャンプ時の加速度
	float jumpAcc_ = 0.0f;

	//合計加速度
	Vector3 totalAxcell_;

	//通常加速度
	const float kMoveAxcellZ_ = 0.5f;

	//右向き加速ベクトル
	Vector3 rightAxcellVec_;

	//加速フラグ
	bool isRightAxcell_;
	//加速時間
	const int32_t kAxcellTime_ = 30;
	//加速計測時間
	int32_t axcellTimer_ = kAxcellTime_;

public:

	bool GetIsMoving() { return isMoving_; }
	bool GetIsFinish() { return isfinish_; }
	bool GetIsDead() { return isDead_; }
	Vector3 GetPos() { return position_; }
	bool GetOnGround() { return onGround_; }
	Attribute GetAttribute() { return attribute_; }
	Vector3 GetPlayerInitPos() { return playerInitPos_; }
	Vector3 GetTotalAxcell() { return totalAxcell_; }


	void SetIsMoving(bool isMoving) { isMoving_ = isMoving; }
	void SetGamePad(GamePad* gamePad) { gamePad_ = gamePad; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }
};

