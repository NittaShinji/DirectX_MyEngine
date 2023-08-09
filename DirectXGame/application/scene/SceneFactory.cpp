#include "SceneFactory.h"
#include "TitleScene.h"
#include "StageSelect.h"
#include "GameScene.h"
#include "ClearScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //éüÇÃÉVÅ[ÉìÇê∂ê¨
    std::unique_ptr<BaseScene> newScene = nullptr;

    if(sceneName == "TITLE")
    {
        newScene = std::make_unique<TitleScene>();
    }
    else if(sceneName == "StageSelect")
    {
        newScene = std::make_unique<StageSelectScene>();
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
