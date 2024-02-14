#pragma once
#include "Vector3.h"
#include "Sprite.h"
#include "Input.h"
#include "GamePad.h"
#include "GameSpeed.h"
#include "ParticleEmitter.h"
#include "Billboard.h"
#include <stdint.h>

/// <summary>
/// イベントクラス
/// 特定の場所において、ゲームスピードや画像を表示する
/// </summary>
class Event
{
public:

	//静的初期化
	static void StaticInitialize(KeyInput* keys, GamePad* gamePad, GameSpeed* gameSpeed);
	//初期化
	void Initialzie(float startPos, float endPos);
	//更新
	void Update(float playerPosZ, GameSpeed::SpeedMode speedMode, int16_t buttonInfo, BYTE keyboardInfo, Camera* camera);
	//描画
	void Draw();
	//画像をセット
	void AddSprite(const std::string& fileName, const Vector2& position,const Vector2& size);
	//ビルボードをセット
	void AddBillboard(const std::string& fileName, const Billboard::BillboardType& billBoardtype, const Vector3& position,float scale,const Vector4& color);
	//リセット
	void Reset();

private:

	static KeyInput* keys_;
	static GamePad* gamePad_;
	static GameSpeed* gameSpeed_;

	//イベントが開始しているかどうか
	bool isStart_;
	//イベントが終了しているかどうか
	bool isFinish_;
	//イベント開始位置
	float startPos_;
	//イベント終了位置
	float finishPos_;
	//イベントボタン画像
	std::vector<std::unique_ptr<Sprite>> eventButtonSprites_;
	//イベントビルボード画像
	std::vector<std::unique_ptr<Billboard>> eventBillboard_;
};

