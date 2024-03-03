#pragma once
#include "TouchableObject.h"
#include "HitWall.h"
#include "GoalOBJ.h"
#include "MirrorOBJ.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"
#include "BackGround.h"
#include "StageBlock.h"

class Camera;
class Player;

/// <summary>
/// ステージクラス
/// (背景オブジェクトや障害物のオブジェクトを管理するクラス)
/// </summary>
class Stage : public TouchableObject
{
public:

	enum stageNum
	{
		tutorialStage = 0,
		Stage1,
	};

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
	void Reset();

	//ImGuiアップデート
	void ImguiUpdate();

	void NextStageUpdate();

	void NextStageLoad();

	void ResetLoad();

	//破壊された壁情報の座標を渡す関数
	std::vector<Vector3> GetBreakWallsPos();

	//ゲッター
	Vector3 GetGoalPos() { return goalPos_; };
	GoalOBJ* GetGoal() { return goal_.get(); }
	int32_t GetStageNum() { return stageNum_; }
	bool GetIsClearedAllStage() { return isClearedAllStage_; }
	bool GetIsAllowedToCountStageNum() { return isAllowedToCountStageNum_; }
	float GetStageEdge() { return stageEdge_; }
	bool GetIsPlayerReachedStageEdge() { return isPlayerReachedStageEdge_; }

	//セッター
	void SetIsAllowedToCountStageNum(bool isAllowed) { isAllowedToCountStageNum_ = isAllowed; }

private:

	const std::string kDefaultStageName_ = "Stage";
	const int32_t kEndStageNum_ = 2;

private:

	//プレイヤー
	Player* player_ = nullptr;

	//ステージ情報
	LevelData* levelData_ = nullptr;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<StageBlock>> stageBlocks_;
	std::vector<std::unique_ptr<HitWall>> walls_;
	std::vector<std::unique_ptr<ResultRoopStage>> resultRoopStages_;
	std::vector<std::unique_ptr<MirrorOBJ>> mirrorRoopObjects_;
	std::vector<std::unique_ptr<MirrorOBJ>> mirrorStageObjects_;
	std::unique_ptr<GoalOBJ> goal_;
	std::unique_ptr<BackGround> blurbackGround_;
	std::unique_ptr<BackGround> normalbackGround_;

	Vector3 kDebugYellowOBJColor_;
	Vector3 kDebugPinkOBJColor_;

	//現在のステージ数
	int32_t stageNum_;
	//ステージの最後をクリアしたか
	bool isClearedAllStage_;

	//ステージの数を増やしても良いか
	bool isAllowedToCountStageNum_;

	//ステージの端
	float stageEdge_;

	//プレイヤーがステージの端まで到達したかどうか
	bool isPlayerReachedStageEdge_;

	//ステージを読み込んだか
	bool canResetLoadedStage_;

	//リセットで読み込むステージ番号
	int resetStageNum_;

	//ゴール座標
	Vector3 goalPos_;
};

