#pragma once
#include "windows.h"
#include "stdint.h"
#pragma warning(push)
#pragma warning(disable:4023)
#include "xinput.h"
#pragma warning(pop)

#pragma comment(lib, "XInput.lib")

/// <summary>
/// XBOXコントローラー用ゲームパッドクラス
/// </summary>
class GamePad
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="padNum">コントローラー番号</param>
	void Initialzie(UINT padNum);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="padNum">コントローラー番号</param>
	/// <returns>接続できたかどうか</returns>
	bool IsConnected(UINT padNum);

	//デッドゾーンの設定
	void CheckDeadZone();

	//振動の設定
	void SetVibration();

	//1フレーム前のボタン情報の保存
	void SaveOldButton();

	//ボタン情報をリセットする
	void ResetButton();

	//押した状態かどうか
	bool HasPushedButton(int16_t button);
	//離している
	bool HasReleasedButton(int16_t button);
	//押した瞬間
	bool PushedButtonMoment(int16_t button);
	//離した瞬間
	bool ReleaseButtonMoment(int16_t button);
	//引数に渡されたビット番号とボタンのビット番号を比較し、同じならボタンフラグをONにする
	bool CompareButton(int16_t bottun);
	//トリガーを押した瞬間かどうか
	bool PushedLeftTriggerMoment();		//左
	bool PushedRightTriggerMoment();	//右

private:

	//トリガーのMax値
	const unsigned char kMaxTriggerNum_ = 255;

	//コントローラの番号
	UINT padNum_;

	//コントローラーの状態
	XINPUT_STATE state_;
	XINPUT_STATE oldState_;

	//入力されたボタンが有効かどうか
	bool isEnabledButton_;

public:

	XINPUT_STATE GetState() { return state_; }
	WORD GetButton() { return state_.Gamepad.wButtons; };
};

