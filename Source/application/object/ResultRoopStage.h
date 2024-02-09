#pragma once
#include "TouchableObject.h"

/// <summary>
/// リザルト時にループするステージ
/// </summary>

struct LevelData;

class ResultRoopStage : public TouchableObject
{

public:

	/// <summary>
	/// ゴールオブジェクトを作成
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="coliderAttribute">コライダー属性</param>
	/// <returns></returns>
	static std::unique_ptr<ResultRoopStage> Create(const std::string& fileName, const unsigned short coliderAttribute);

	///// <summary>
	///// 他のOBJとの衝突時に呼ばれる関数
	///// </summary>
	///// <param name="info">衝突した相手の情報</param>
	//void OnCollision(const CollisionInfo& info) override;

	//初期化
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="playerPos">プレイヤー座標</param>
	void Update(Camera* camera, Vector3 playerPos,float roopArea);

	//描画
	void Draw() override;

	//リセット
	void Reset();

	//ループ用に座標をリセットする
	void SetRoopPosition();

private:

	float objectDistance_ = 20.0f;
	static const float objectRadius;

private:

	//ループオブジェクトの数
	static int32_t roopObjectNum_;

	//ループした回数
	int32_t roopCount_;

	//ループできるか
	bool isRoop_;

	//全体のループフラグ
	static bool isFinishedRoopObjects_;
	
	float initTransFormZ_;

public: //アクセッサ

	static void SetRoopObjectNum(int32_t roopObjectNum) { roopObjectNum_ = roopObjectNum; }
	void SetInitTransFormZ(float initTransFormZ) { initTransFormZ_ = initTransFormZ; }
	void SetRoopCount(int32_t roopCount) { roopCount_ = roopCount; }
	void SetIsRoop(bool isRoop) { isRoop_ = isRoop; }
	static const float GetObjectRadius() { return objectRadius; }

	static void SetIsFinishedRoopObjects(bool isFinishedRoopObjects) { isFinishedRoopObjects_ = isFinishedRoopObjects; }
	static int32_t GetRoopObjectNum() { return roopObjectNum_; }
	bool GetIsRoop() { return isRoop_; }
};

