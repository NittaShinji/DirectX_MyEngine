#pragma once
#include "Sprite.h"
class GameTimer
{
public: 

	void Initialize();
	void Update();
	void Reset();
	void LoadNumTexture();

private:

	//ゲーム中に表示する時間(4桁)
	float inGameTime_;
	//クリア画面で表示する時間(6桁)
	float resultTime_;
	//ハイスコア
	float highScoreTime_;

	std::unique_ptr<Sprite> clock_;
	std::unique_ptr<Sprite> num[10];
};

