#pragma once
#include "BaseScene.h"
#include "Sprite.h"

class TitleScene : public BaseScene
{
public:

	static void StaticInitialize();

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//終了
	void Finalize() override;

	//描画
	void Draw() override;

private:

	std::unique_ptr<Sprite> title_ = nullptr;

	static KeyInput* keys_;
};

