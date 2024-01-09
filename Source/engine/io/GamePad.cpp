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
}

bool GamePad::HasPushedButton(int16_t button)
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

bool GamePad::HasReleasedButton(int16_t button)
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

bool GamePad::PushedButtonMoment(int16_t button)
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

bool GamePad::ReleaseButtonMoment(int16_t button)
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

