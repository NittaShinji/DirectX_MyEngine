#pragma once
#include "BaseScene.h"

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

	//次のシーンを予約
	void SetNextScene(std::unique_ptr<BaseScene> nextScene) { nextScene_ = std::move(nextScene); }

private:

	SceneManager();
	~SceneManager();
	//コピーコンストラクタの無効
	SceneManager(const SceneManager& sceneManager) = delete;
	//代入演算子の無効
	SceneManager& operator=(const SceneManager& sceneManager) = delete;

	//次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;
	//今のシーン(実行中シーン)
	std::unique_ptr<BaseScene> scene_ = nullptr;

};

