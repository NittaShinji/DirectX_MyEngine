#include "GamePad.h"
#include <wrl.h>

void GamePad::Initialzie(UINT padNum)
{
	padNum_ = padNum - 1;
}

bool GamePad::IsConnected(UINT padNum)
{
	DWORD dwResult;

	//�ߋ�����ۑ�
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

	//�R���g���[���[���ǂ��܂ŉ�����Ă��邩
	float magnitude = static_cast<float>(sqrt(LX * LX + LY * LY));
	//�R���g���[���[�����������������肷��B
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//�R���g���[�����~�`�̃f�b�h�]�[���̊O�ɂ��邩�ǂ������`�F�b�N����B
	if(magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		//�\�z�����ő�l�ő傫�����N���b�v����B
		if(magnitude > 32767) magnitude = 32767;
		//�f�b�h�]�[���̏I������ɑ傫���𒲐�����
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//�I�v�V�����ŁA�\�z�����͈͂Ɋւ��đ傫���𐳋K������B
		//0.0�`1.0�̃}�O�j�`���[�h��^����B
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	//�R���g���[���[���f�b�h�]�[���ɂ���ꍇ�́A�}�O�j�`���[�h���[���ɂ���B
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
	//1�t���[���O�̏���ۑ�����
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