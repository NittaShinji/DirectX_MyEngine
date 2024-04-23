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
	void Update(float playerPosZ, GameSpeed::SpeedMode speedMode, int16_t buttonInfo, BYTE keyboardInfo, Camera* camera,int32_t canJumpCount);
	//描画
	void Draw();

	//アニメーション更新
	void AnimationUpdate();
	//アニメーション描画
	void AnimationDraw();

	//画像をセット
	void AddSprite(const std::string& fileName, const Vector2& position,const Vector2& size);
	//ビルボードをセット
	void AddBillboard(const std::string& fileName, const Billboard::BillboardType& billBoardtype, const Vector3& position,float scale,const Vector4& color);
	//リセット
	void Reset();
	//イベント終了
	void Finish();

	//画像行列更新
	void SpriteMatUpdate();
	//ビルボード行列更新
	void BillboardMatUpdate(Camera* camera);

	//ビルボード表示
	void UpdateToDisplayBillboard(float playerPosZ, Camera* camera);

	//アニメーションをするかどうかをセット
	void SetIsAnimate(bool isAnimate) { isAnimate_ = isAnimate; }
	//アニメーション時間をセット
	void SetButtonAnimeTime(int32_t buttonAnimeTime) { buttonAnimeTime_ = buttonAnimeTime; }

	//画像をプレイヤーのZ座標位置にセット
	void BillboardSetPlayerPos(const Vector3& playerPos, Camera* camera);
	//透過する
	void TransmissiveBillboard();

	bool GetIsFinish() {return isFinish_; }
	void SetIsFinish(bool isFinish) { isFinish_ = isFinish; }

	const float GetStartPosZ() { return startPos_; }
	const float GetFinishPosZ() { return finishPos_; }

private:

	static KeyInput* keys_;
	static GamePad* gamePad_;
	static GameSpeed* gameSpeed_;

	//トリガーとなる入力情報
	int16_t buttonInfo_;
	BYTE keyboardInfo_;

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

	//アニメーションするかどうか
	bool isAnimate_;

	//通常アニメーション時間
	const int32_t defaultAnimeTime = 17;
	//イベント中にボタンが押されたか
	bool isPushedButton_;
	//ボタンを押し戻すアニメーション用のタイマー
	int32_t buttonTimer_;
	//アニメーション用の時間
	int32_t buttonAnimeTime_;

	//画像が透明になったかどうか
	bool isClearSprite_;
	//ビルボードだけを表示するかどうか
	bool isDisplayBillboard_;
};

