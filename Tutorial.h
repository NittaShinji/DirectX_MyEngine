#pragma once
#include "Camera.h"

class Tutorial
{
public:

	Tutorial();
	~Tutorial();

	//初期化
	void Initialize();

	//毎フレーム更新
	void Update();

	//描画
	void Draw();

private:

	//スプライト強調フラグ
	bool isShowSpriteA;

};

