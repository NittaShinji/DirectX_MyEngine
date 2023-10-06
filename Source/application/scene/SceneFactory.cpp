#include "SceneFactory.h"
#include "TitleScene.h"
#include "StageSelect.h"
#include "TutorialScene.h"
#include "GameScene.h"
#include "ClearScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //次のシーンを生成
    std::unique_ptr<BaseScene> newScene = nullptr;

    if(sceneName == "TITLE")
    {
        newScene = std::make_unique<TitleScene>();
    }
    else if(sceneName == "StageSelect")
    {
        newScene = std::make_unique<StageSelectScene>();
    }
    else if(sceneName == "Tutorial")
    {
        newScene = std::make_unique<TutorialScene>();
    }
    else if(sceneName == "GAME")
    {
        newScene = std::make_unique<GameScene>();
    }
    else if(sceneName == "CLEAR")
    {
        newScene = std::make_unique<ClearScene>();
    }

    return newScene;
}
