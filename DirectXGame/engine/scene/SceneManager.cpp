#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	scene_->Finalize();
}

void SceneManager::Initialize()
{


}

void SceneManager::Update()
{
	//シーン切替機構
	if(nextScene_)
	{
		//旧シーンの終了
		if(scene_)
		{
			scene_->Finalize();
		}

		//シーン切替
		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;

		//シーンマネージャをセット
		scene_->SetSceneManager(this);

		//次シーンを初期化する
		scene_->Initialize();
	}

	//実行中シーンを更新する
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次シーンを生成
	nextScene_ = std::move(sceneFactory_->CreateScene(sceneName));
}
