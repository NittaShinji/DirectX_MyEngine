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
	void Update(Camera* camera);
	//描画
	void Draw();
	//リセット
	void Reset();
	
private: //メンバ変数

	Player* player_ = nullptr;

	//スタートイベント
	std::unique_ptr<Event> startEvent_ = nullptr;
	//ジャンプイベント
	std::unique_ptr<Event> jumpEvent_ = nullptr;
	//二回ジャンプするイベント
	std::unique_ptr<Event> doubleJumpEvent_ = nullptr;
	//ロングジャンプ手前イベント
	std::unique_ptr<Event> heighJumpBeforeEvent_ = nullptr;
	//ロングジャンプ後イベント
	std::unique_ptr<Event> heighJumpAfterEvent_ = nullptr;

	//色変えイベント
	std::unique_ptr<Event> changeColorEvent_ = nullptr;
	//空中での色変えイベント
	std::unique_ptr<Event> changeInAirEvent_ = nullptr;
	//色変えするためのジャンプイベント
	std::unique_ptr<Event> jumptoChangeEvent_ = nullptr;
	//ジャンプ後の色変えイベント
	std::unique_ptr<Event> ChangeColorEvent_ = nullptr;

};

