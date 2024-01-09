#include "GamePad.h"
#include <cmath>
#include <wrl.h>

void GamePad::Initialzie(UINT padNum)
{
	padNum_ = padNum;
}

bool GamePad::IsConnected(UINT padNum)
{
	DWORD dwResult;

	//過去情報を保存
	oldState_ = state_;

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

void GamePad::ResetButton()
{
	isEnabledButton_ = false;
	padButton_.A = false;
	padButton_.B = false;
	padButton_.X = false;
	padButton_.Y = false;
	padButton_.Left = false;
	padButton_.Right = false;
	padButton_.Up = false;
	padButton_.Down = false;
	padButton_.LB = false;
	padButton_.LT = false;
	padButton_.RB = false;
	padButton_.RT = false;
}

//押した状態かどうか
void GamePad::HasPushedButton()
{
	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			padButton_.A = true;
		}
		else
		{
			padButton_.A = false;
		}
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			padButton_.B = true;
		}
		else
		{
			padButton_.B = false;
		}
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			padButton_.X = true;
		}
		else
		{
			padButton_.X = false;
		}
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		padButton_.Y = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		padButton_.Up = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		padButton_.Down = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		padButton_.Left = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		padButton_.Right = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		padButton_.Up = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		padButton_.Down = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		padButton_.Left = true;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		padButton_.Right = true;
	}

}

//離した状態かどうか
void GamePad::HasReleasedButton()
{
	if(!(state_.Gamepad.wButtons & XINPUT_GAMEPAD_A))
	{
		if(!(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_A))
		{
			padButton_.A = true;
		}
		else
		{
			padButton_.A = false;
		}
	}
	else
	{
		padButton_.A = false;
	}

	if(!(state_.Gamepad.wButtons & XINPUT_GAMEPAD_B))
	{
		if(!(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_B))
		{
			padButton_.B = true;
		}
		else
		{
			padButton_.B = false;
		}
	}
	else
	{
		padButton_.B = false;
	}
}

//押した瞬間かどうか
void GamePad::PushedButtonMoment()
{
	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if(!(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_A))
		{
			padButton_.A = true;
		}
		else
		{
			padButton_.A = false;
		}
	}
	else
	{
		padButton_.A = false;
	}

	if(state_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		if(!(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_B))
		{
			padButton_.B = true;
		}
		else
		{
			padButton_.B = false;
		}
	}
	else
	{
		padButton_.B = false;
	}
}

//離した瞬間かどうか
void GamePad::ReleaseButtonMoment()
{
	if(!(state_.Gamepad.wButtons & XINPUT_GAMEPAD_A))
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			padButton_.A = true;
		}
		else
		{
			padButton_.A = false;
		}
	}
	else
	{
		padButton_.A = false;
	}

	if(!(state_.Gamepad.wButtons & XINPUT_GAMEPAD_B))
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			padButton_.B = true;
		}
		else
		{
			padButton_.B = false;
		}
	}
	else
	{
		padButton_.B = false;
	}

	if(!(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			padButton_.Right = true;
		}
		else
		{
			padButton_.Right = false;
		}
	}
	else
	{
		padButton_.Right = false;
	}

	if(!(state_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
	{
		if(oldState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			padButton_.Left = true;
		}
		else
		{
			padButton_.Left = false;
		}
	}
	else
	{
		padButton_.Left = false;
	}
}

bool GamePad::HasPushedButton1(int16_t button)
{
	bool result = false;

	if(state_.Gamepad.wButtons & button)
	{
		if(oldState_.Gamepad.wButtons & button)
		{
			result = CompareButton(button);
		}
		else
		{
			isEnabledButton_ = false;
		}
	}
	else
	{
		isEnabledButton_ = false;
	}

	return result;
}

bool GamePad::HasReleasedButton1(int16_t button)
{
	bool result = false;

	if(!(state_.Gamepad.wButtons & button))
	{
		if(!(oldState_.Gamepad.wButtons & button))
		{
			result = CompareButton(button);
		}
		else
		{
			isEnabledButton_ = false;
		}
	}
	else
	{
		isEnabledButton_ = false;
	}

	return result;
}

bool GamePad::PushedButtonMoment1(int16_t button)
{
	bool result = false;

	if(state_.Gamepad.wButtons & button)
	{
		if(!(oldState_.Gamepad.wButtons & button))
		{
			result = CompareButton(button);
		}
		else
		{
			isEnabledButton_ = false;
		}
	}
	else
	{
		isEnabledButton_ = false;
	}

	return result;
}

bool GamePad::ReleaseButtonMoment1(int16_t button)
{
	bool result = false;

	if(!(state_.Gamepad.wButtons & button))
	{
		if(oldState_.Gamepad.wButtons & button)
		{
			result = CompareButton(button);
		}
		else
		{
			isEnabledButton_ = false;
		}
	}
	else
	{
		isEnabledButton_ = false;
	}

	return result;
}

bool GamePad::CompareButton(int16_t button)
{
	if(button == XINPUT_GAMEPAD_A)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_B)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_X)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_Y)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_DPAD_UP)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_DPAD_DOWN)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_DPAD_LEFT)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_LEFT_THUMB)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		isEnabledButton_ = true;
	}
	else if(button == XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		isEnabledButton_ = true;
	}

	return isEnabledButton_;
}

bool GamePad::HasPushedButtonA()
{
	HasPushedButton();
	return padButton_.A;
}

bool GamePad::ReleaseButtonMomentA()
{
	ReleaseButtonMoment();
	return padButton_.A;
}

bool GamePad::PushedButtonMomentA()
{
	PushedButtonMoment();
	return padButton_.A;
}

bool GamePad::ReleaseButtonMomentB()
{
	ReleaseButtonMoment();
	return padButton_.B;
}

bool GamePad::PushedButtonMomentB()
{
	PushedButtonMoment();
	return padButton_.B;
}

bool GamePad::HasPushedButtonB()
{
	HasPushedButton();
	return padButton_.B;
}

bool GamePad::ReleaseButtonMomentRight()
{
	ReleaseButtonMoment();
	return padButton_.Right;
}

bool GamePad::PushedButtonMomentRight()
{
	PushedButtonMoment();
	return padButton_.Right;
}

bool GamePad::HasPushedButtonRight()
{
	HasPushedButton();
	return padButton_.Right;
}

bool GamePad::ReleaseButtonMomentLeft()
{
	ReleaseButtonMoment();
	return padButton_.Left;
}

bool GamePad::PushedButtonMomentLeft()
{
	PushedButtonMoment();
	return padButton_.Left;
}

bool GamePad::HasPushedButtonLeft()
{
	HasPushedButton();
	return padButton_.Left;
}
