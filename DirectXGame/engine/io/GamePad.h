#pragma once
#include "windows.h"
#include <directXMath.h>
#include "xinput.h"

#pragma comment(lib, "XInput.lib")

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

	void Initialzie(UINT padNum);

	//�X�V
	bool IsConnected(UINT padNum);

	//�f�b�h�]�[���̐ݒ�
	void CheckDeadZone();

	//�U���̐ݒ�
	void SetVibration();

	XINPUT_STATE GetState() {return state_; }
	WORD GetButton() { return state_.Gamepad.wButtons; };

	void SaveOldButton();

	//�e�{�^���̔���
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

	//�{�^���������Z�b�g����
	void ResetButton();

	//�������u��
	void HasPushedButton();

	//�������u��
	void HasReleasedButton();

	//���������Ă���
	void PushedButtonMoment();

	//�����Ă���
	void ReleaseButtonMoment();

	PadButton GetPadButton() { return padButton_; }

private:

	bool availability[XUSER_MAX_COUNT];
	XINPUT_STATE state_;
	XINPUT_STATE oldState_;

	//�R���g���[���̔ԍ�
	UINT padNum_;

	//�e�{�^���̍\����
	PadButton padButton_;

};

