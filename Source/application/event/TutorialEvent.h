#pragma once
#include "Sprite.h"
#include "GameSpeed.h"
#include "GamePad.h"
#include "Event.h"

class Player;

/// <summary>
/// チュートリアルイベント
/// </summary>
class TutorialEvent
{
public:

	//初期化
	void Initialzie(Player* player);
	//更新
	void Update();
	//描画
	void Draw();
	
private: //メンバ変数

	Player* player_ = nullptr;

	//ジャンプイベント
	std::unique_ptr<Event> jumpEvent_ = nullptr;
	//ハイジャンプイベント
	std::unique_ptr<Event> hightJumpEvent_ = nullptr;
	//色変えイベント
	std::unique_ptr<Event> changeColorEvent_ = nullptr;
	//空中での色変えイベント
	std::unique_ptr<Event> changeInAirEvent_ = nullptr;
};

