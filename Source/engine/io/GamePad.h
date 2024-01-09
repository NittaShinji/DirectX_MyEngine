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

	typedef struct PadButton
	{
		bool A;
		bool B;
		bool Y;
		bool X;
		bool Left;
		bool Right;
		bool Up;
		bool Down;
		bool LB;
		bool LT;
		bool RB;
		bool RT;
	} PadButton;

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

	//各ボタンの判定を返す関数
	bool GetButtonA() { return padButton_.A; };
	bool GetButtonB() { return padButton_.B; }
	bool GetButtonX() { return padButton_.X; }
	bool GetButtonY() { return padButton_.Y; }
	bool GetButtonLeft() { return padButton_.Left; }
	bool GetButtonRight() { return padButton_.Right; }
	bool GetButtonUp() { return padButton_.Up; }
	bool GetButtonDown() { return padButton_.Down; }
	bool GetButtonLB() { return padButton_.LB; }
	bool GetButtonLT() { return padButton_.LT; }
	bool GetButtonRB() { return padButton_.RB; }
	bool GetButtonRT() { return padButton_.RT; }

	//ボタン情報をリセットする
	void ResetButton();

	//押した状態かどうか
	void HasPushedButton();

	//離している
	void HasReleasedButton();

	//押した瞬間
	void PushedButtonMoment();

	//離した瞬間
	void ReleaseButtonMoment();

	//押した状態かどうか
	bool HasPushedButton1(int16_t button);
	//離している
	bool HasReleasedButton1(int16_t button);
	//押した瞬間
	bool PushedButtonMoment1(int16_t button);
	//離した瞬間
	bool ReleaseButtonMoment1(int16_t button);

	//引数に渡されたビット番号とボタンのビット番号を比較し、同じならボタンフラグをONにする
	bool CompareButton(int16_t bottun);

	//Aボタンを離した瞬間か
	bool ReleaseButtonMomentA();
	//Aボタンを押した瞬間か
	bool PushedButtonMomentA();
	//Aボタンを押し続けているか
	bool HasPushedButtonA();

	//Bボタンを離した瞬間か
	bool ReleaseButtonMomentB();
	//Bボタンを押した瞬間か
	bool PushedButtonMomentB();
	//Bボタンを押し続けているか
	bool HasPushedButtonB();

	//右ボタンが離した瞬間か
	bool ReleaseButtonMomentRight();
	//右ボタンを押した瞬間か
	bool PushedButtonMomentRight();
	//右ボタンを押し続けているか
	bool HasPushedButtonRight();

	//右ボタンが離した瞬間か
	bool ReleaseButtonMomentLeft();
	//右ボタンを押した瞬間か
	bool PushedButtonMomentLeft();
	//右ボタンを押し続けているか
	bool HasPushedButtonLeft();


private:

	//コントローラの番号
	UINT padNum_;

	//コントローラーの状態
	XINPUT_STATE state_;
	XINPUT_STATE oldState_;

	//各ボタンの構造体
	PadButton padButton_;

	//入力されたボタンが有効かどうか
	bool isEnabledButton_;

public:

	//アクセッサ
	PadButton GetPadButton() { return padButton_; }
	XINPUT_STATE GetState() { return state_; }
	WORD GetButton() { return state_.Gamepad.wButtons; };
};

