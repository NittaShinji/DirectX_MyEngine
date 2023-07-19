#pragma once
#include "BaseScene.h"

class SceneManager
{

public: //メンバ関数

	SceneManager();

	~SceneManager();

	void Initialize();

	void Update();

	void Draw();

	//次のシーンを予約
	void SetNextScene(std::unique_ptr<BaseScene> nextScene) { nextScene_ = std::move(nextScene); }

private:

	//次のシーン
	//BaseScene* nextScene_ = nullptr;
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	//今のシーン(実行中シーン)
	//BaseScene* scene_ = nullptr;
	std::unique_ptr<BaseScene> scene_ = nullptr;

};

