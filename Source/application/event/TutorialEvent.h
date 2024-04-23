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

	enum class EventSavePoint
	{
		First,
		LongJump,
		ColorChange,
		JumpColorChange,
		Finish,
	};

public:

	//初期化
	void Initialzie(Player* player, Camera* camera);
	//更新
	void Update();
	//描画
	void Draw();
	//全てのイベントをリセット
	void AllEventReset();
	//セーブポイントに沿ったイベントをリセット
	void ResetEventforSavePoint();

	void CalcSavePoint(Event* event);

	void SetEventSavePoint(EventSavePoint eventSavePoint) { eventSavePoint_ = eventSavePoint; }

public:

	//中間地点の数
	static const int32_t kMaxIntermediateEventNum = 3;
	
private: //メンバ変数

	Player* player_ = nullptr;
	Camera* camera_ = nullptr;

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
	std::unique_ptr<Event> firstChangeColorEvent_ = nullptr;
	//空中での色変えイベント
	std::unique_ptr<Event> changeInAirEvent_ = nullptr;
	//色変えするためのジャンプイベント
	std::unique_ptr<Event> jumptoChangeEvent_ = nullptr;
	//ジャンプ後の色変えイベント
	std::unique_ptr<Event> changeColorEvent_ = nullptr;
	//加速イベント
	std::unique_ptr<Event> axcellEvent_ = nullptr;

	//現在どこまでステージをクリアしたかを保存
	EventSavePoint eventSavePoint_;
};

