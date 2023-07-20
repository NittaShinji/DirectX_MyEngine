#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{

public: //メンバ関数

	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}

	void Initialize();

	void Update();

	void Draw();

	/// <summary>
	/// シーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	//シーンファクトリーのセッター
	//void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory> sceneFactory) { sceneFactory_ = std::move(sceneFactory); }


private:

	SceneManager();
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

