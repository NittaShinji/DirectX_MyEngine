#pragma once
#include "Sprite.h"
#include "GameSpeed.h"

class Player;

class TutorialEvent
{
public:

	//初期化
	void Initialzie();
	//更新
	void Update(Player* player,GameSpeed* gameSpeed);
	//描画
	void Draw();

private: // 定数

	//イベント開始地点
	enum eventPos
	{
		StartJumpEventPos = 100,
		EndJumpEventPos = 300,
	};

private: //メンバ変数

	//ジャンプ画像
	std::unique_ptr<Sprite> aButtonSprite_ = nullptr;

	//イベント開始フラグ
	bool isStartJumpEvent_;

};

