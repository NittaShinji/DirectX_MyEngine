#pragma once
#include "TouchableObject.h"
#include "LevelManager.h"
#include "ObjectAttribute.h"

class HitWall : public TouchableObject
{
public:

	/// <summary>
	/// 壁オブジェクトを作成
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	static std::unique_ptr<HitWall> Create(const std::string& fileName);

	/// <summary>
	/// 他のOBJとの衝突時に呼ばれる関数
	/// </summary>
	/// <param name="info">衝突した相手の情報</param>
	void OnCollision(const CollisionInfo& info) override;

	//初期化
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="isPlayerAccelerating">プレイヤーが加速しているかどうか</param>
	void Update(Camera* camera, bool isPlayerAccelerating);

	//描画
	void Draw() override;

	//アクセッサ
	bool GetIsBreak_() { return isBreak_; }
	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }
	Vector3 GetWallPos() { return transform_; }

private:

	//壁が壊れているか
	bool isBreak_;

	//プレイヤーが加速しているか
	bool isPlayerAccelerating_;
};

