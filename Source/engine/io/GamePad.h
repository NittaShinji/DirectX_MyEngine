#pragma once
#include "windows.h"
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

	//押した瞬間
	void HasPushedButton();

	//離した瞬間
	void HasReleasedButton();

	//押し続けている
	void PushedButtonMoment();

	//離している
	void ReleaseButtonMoment();

private:

	//コントローラの番号
	UINT padNum_;

	//コントローラーの状態
	XINPUT_STATE state_;
	XINPUT_STATE oldState_;

	//各ボタンの構造体
	PadButton padButton_;

public:

	//アクセッサ
	PadButton GetPadButton() { return padButton_; }
	XINPUT_STATE GetState() { return state_; }
	WORD GetButton() { return state_.Gamepad.wButtons; };
};

