#pragma once
#include "BaseScene.h"
#include "Sprite.h"

class ClearScene : public BaseScene
{

public:

	//初期化
	void Initialize() override;

	//毎フレーム更新
	void Update() override;

	//描画
	void Draw() override;

private:

	std::unique_ptr<Sprite> end_ = nullptr;

	static KeyInput* keys_;
};

