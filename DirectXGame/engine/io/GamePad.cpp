#include "GamePad.h"
#include <wrl.h>

void GamePad::Initialzie(UINT padNum)
{
	padNum_ = padNum - 1;
}

bool GamePad::IsConnected(UINT padNum)
{
	DWORD dwResult;

	//過去情報を保存
	//oldState_ = state_;

	ZeroMemory(&state_, sizeof(XINPUT_STATE));

	dwResult = XInputGetState(padNum, &state_);

	if(dwResult == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GamePad::CheckDeadZone()
{
	float LX = state_.Gamepad.sThumbLX;
	float LY = state_.Gamepad.sThumbLY;

	//コントローラーがどこまで押されているか
	float magnitude = static_cast<float>(sqrt(LX * LX + LY * LY));
	//コントローラーが押される方向を決定する。
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//コントローラが円形のデッドゾーンの外にあるかどうかをチェックする。
	if(magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		//予想される最大値で大きさをクリップする。
		if(magnitude > 32767) magnitude = 32767;
		//デッドゾーンの終わりを基準に大きさを調整する
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//オプションで、予想される範囲に関して大きさを正規化する。
		//0.0～1.0のマグニチュードを与える。
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	//コントローラーがデッドゾーンにある場合は、マグニチュードをゼロにする。
	else
	{
		magnitude = 0.0f;
		normalizedMagnitude = 0.0f;
	}
}

void GamePad::SetVibration()
{
	for(DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = 32000;
		vibration.wRightMotorSpeed = 16000;
		XInputSetState(i, &vibration);
	}
}

void GamePad::SaveOldButton()
{
	//1フレーム前の情報を保存する
	oldState_ = state_;
}

void GamePad::HasPushedButton()
{
	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_A)
	{
		padButton_.A = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_B)
	{
		padButton_.B = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_X)
	{
		padButton_.X = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_Y)
	{
		padButton_.Y = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_UP)
	{
		padButton_.Up = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_DOWN)
	{
		padButton_.Down = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_LEFT)
	{
		padButton_.Left = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		padButton_.Right = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_UP)
	{
		padButton_.Up = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_DOWN)
	{
		padButton_.Down = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_LEFT)
	{
		padButton_.Left = true;
	}

	if(state_.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		padButton_.Right = true;
	}

}

void GamePad::HasReleasedButton()
{
}

void GamePad::PushedButtonMoment()
{
}

void GamePad::ReleaseButtonMoment()
{
}