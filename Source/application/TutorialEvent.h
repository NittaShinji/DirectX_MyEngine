#pragma once
#include "Sprite.h"
#include "GameSpeed.h"
#include "GamePad.h"
#include "Event.h"

class Player;

class TutorialEvent
{
public:

	//初期化
	void Initialzie();
	//更新
	void Update(Player* player);
	//描画
	void Draw();
	
private: //メンバ変数

	//ジャンプイベント
	std::unique_ptr<Event> jumpEvent_ = nullptr;
};

