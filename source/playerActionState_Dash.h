/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�_�b�V���X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Dash.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_Dash : public PlayerActionStateBase
{
private:
	static inline const float m_DASH_MAGNITUDE = 0.3f;	// �_�b�V�������x
	static inline const float m_INPUT_END_TIME = 0.2f;

	bool m_IsMoveInputX = false;
	bool m_IsMoveInputZ = false;
	float m_InputEndTimeCount = m_INPUT_END_TIME;

public:
	PlayerActionState_Dash(PlayerManager* Manager);
	~PlayerActionState_Dash() {}

	void Update() override;
	void LateUpdate() override;

	void ExecMove_Z(PlayerInputListener::ActionElement* Elem) override;
	void ExecMove_X(PlayerInputListener::ActionElement* Elem) override;

	void CheckEnd();	// �_�b�V���̏I�������𖞂������m�F
};
