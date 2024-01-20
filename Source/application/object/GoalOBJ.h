#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

/// <summary>
/// 当たり判定のあるゴールオブジェクト
/// </summary>

class GameSpeed;

class GoalOBJ : public TouchableObject
{
public:

	/// <summary>
	/// ゴールオブジェクトを作成
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="coliderAttribute">コライダー属性</param>
	/// <returns></returns>
	static std::unique_ptr<GoalOBJ> Create(const std::string& fileName, const unsigned short coliderAttribute);

	/// <summary>
	/// 他のOBJとの衝突時に呼ばれる関数
	/// </summary>
	/// <param name="info">衝突した相手の情報</param>
	void OnCollision(const CollisionInfo& info) override;
	
	//初期化
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="playerPos">プレイヤー座標</param>
	void Update(Camera* camera, Vector3 playerPos);

	//描画
	void Draw() override;

	//ゴールに近づいたらゲームスピードをスローにする
	void SlowDownNearGoal(GameSpeed* gameSpeed, bool isFinish);

	//アクセッサ
	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }
	void SetIsStartGoalStagin(bool isStartGoalStagin) { isStartGoalStagin_ = isStartGoalStagin; }
	bool GetIsBreak_() { return isBreak_; }
	bool GetIsStartGoalStagin() { return isStartGoalStagin_; }
	Vector3 GetWallPos() { return transform_; }

private:

	//プレイヤーが近づいた際に演出を入れる処理
	const float kStagingStartDistance_ = 7.0f;

	//演出開始フラグ
	bool isStartGoalStagin_ = false;

	//壁が壊れているか
	bool isBreak_;

	//プレイヤーが加速しているか
	bool isPlayerAccelerating_;
};


