#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// シーンマネージャー
/// シーンを管理する
/// </summary>
class SceneManager final
{

public: //メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

	/// <summary>
	/// シーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	//シーンファクトリーのセッター
	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory> sceneFactory) { sceneFactory_ = std::move(sceneFactory); }

private:

	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();
	//コピーコンストラクタの無効
	SceneManager(const SceneManager& sceneManager) = delete;
	//代入演算子の無効
	SceneManager& operator=(const SceneManager& sceneManager) = delete;

	//シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
	//次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;
	//今のシーン(実行中シーン)
	std::unique_ptr<BaseScene> scene_ = nullptr;
};

