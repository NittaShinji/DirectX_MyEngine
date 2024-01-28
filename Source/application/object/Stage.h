#pragma once
#include "TouchableObject.h"
#include "HitWall.h"
#include "GoalOBJ.h"
//#include "ResultRoopStage.h"
#include "MirrorOBJ.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class Camera;
class Player;

/// <summary>
/// ステージクラス
/// (背景オブジェクトや障害物のオブジェクトを管理するクラス)
/// </summary>
class Stage : public TouchableObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="player">プレイヤー</param>
	void Update(Camera* camera, Player* player, GameSpeed* gameSpeed);

	void Load();

	//描画
	void Draw();

	//ミラー描画
	void MirrorDraw();

	/// <summary>
	/// リセット
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void Reset(const std::string& fileName);

	//ImGuiアップデート
	void ImguiUpdate();

	void NextStageUpdate();

	//破壊された壁情報の座標を渡す関数
	std::vector<Vector3> GetBreakWallsPos();

	//ゲッター
	Vector3 GetGoalPos() { return goalPos_; };
	GoalOBJ* GetGoal() { return goal_.get(); }

private:

	const std::string kDefaultStageName_ = "Stage";

private:

	//プレイヤー
	Player* player_ = nullptr;

	//ステージ情報
	LevelData* levelData_ = nullptr;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<TouchableObject>> objects_;
	std::vector<std::unique_ptr<HitWall>> walls_;
	std::vector<std::unique_ptr<ResultRoopStage>> resultRoopStages_;
	std::vector<std::unique_ptr<MirrorOBJ>> mirrorObjects_;
	std::unique_ptr<GoalOBJ> goal_;

	Vector3 kDebugYellowOBJColor_;
	Vector3 kDebugPinkOBJColor_;

	//現在のステージ数
	int32_t stageNum_;

	//ステージの端
	float stageEdge_;

	//ゴール座標
	Vector3 goalPos_;
};

