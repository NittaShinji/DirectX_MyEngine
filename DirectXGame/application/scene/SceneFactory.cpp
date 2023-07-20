#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //Ÿ‚ÌƒV[ƒ“‚ğ¶¬
    std::unique_ptr<BaseScene> newScene = nullptr;

    if(sceneName == "TITLE")
    {
        newScene = std::make_unique<TitleScene>();
    }
    else if(sceneName == "GAME")
    {
        newScene = std::make_unique<GameScene>();
    }

    return newScene;
}
