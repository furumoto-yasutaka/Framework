/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͎�t�X�N���v�g
*	�t�@�C���F	playerInputListener_Controller.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerInputListener_Controller.h"
#include "input.h"

float PlayerInputListener_Controller::InputMove_Z()
{
	return Input::GetGamePadStickLeftY();
}

float PlayerInputListener_Controller::InputMove_X()
{
	return Input::GetGamePadStickLeftX();
}

bool PlayerInputListener_Controller::InputJump()
{
	return Input::GetGamePadButtonTrigger(XINPUT_GAMEPAD_A);
}

bool PlayerInputListener_Controller::InputAvoid()
{
	return Input::GetGamePadButtonTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER);
}

optional<float> PlayerInputListener_Controller::InputRoll_L()
{
	optional<float> value = Input::GetGamePadLeftTrigger();
	return value > 0.0f ? value : nullopt;
}

optional<float> PlayerInputListener_Controller::InputRoll_R()
{
	optional<float> value = Input::GetGamePadRightTrigger();
	return value > 0.0f ? value : nullopt;
}

bool PlayerInputListener_Controller::InputWeaponDeformation()
{
	return Input::GetGamePadButtonTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER);
}
