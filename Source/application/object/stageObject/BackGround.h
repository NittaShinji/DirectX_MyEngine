#pragma once
#include"Object3d.h"
#include "Model.h"
#include "Camera.h"
#include "LevelManager.h"

/// <summary>
/// 背景オブジェクトクラス
/// </summary>
class BackGround : public Object3d
{
public:

	BackGround();
	~BackGround();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void Initialize(const std::string fileName, float stageEdge);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update(Camera* camera);

	//描画
	void Draw();

	//読み込み
	void Load(const std::string fileName,float stageEdge);

	//背景オブジェクトをクリア
	void Clear();

	//位置を初期座標にリセット
	void ResetPos();

private:

	//ステージ情報
	LevelData* levelData_ = nullptr;

	//blender読み込みオブジェクト
	std::vector<std::unique_ptr<Object3d>> objects_;
};

