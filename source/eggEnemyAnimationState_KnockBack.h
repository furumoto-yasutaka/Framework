/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�m�b�N�o�b�N�A�j���[�V�����N���X
*	�t�@�C���F	eggEnemyAnimationState_KnockBack.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyAnimationStateBase.h"

class EggEnemyAnimationState_KnockBack : public EggEnemyAnimationStateBase
{
private:
	static inline const float m_ROTATE_ANGLE = 30.0f;							// �X���p�x
	static inline const float m_ROTATE_ANGLE_INTERPOLATION_RATE = 0.2f;			// 1�t���[��������̌X���̕�Ԋ���
	static inline const float m_ROTATE_ANGLE_INTERPOLATION_THRESHOLD = 0.01f;	// ��ԏI���̂������l

	static inline const float m_END_ROTATE_ANGLE_INTERPOLATION_RATE = 0.1f;		// 1�t���[��������̌X���̕�Ԋ���

	float m_RotatedAngle = 0.0f;
	bool m_IsEndAnimation = false;

public:
	EggEnemyAnimationState_KnockBack(EggEnemyManager* Manager, AnimationBaseParam Parameter);
	~EggEnemyAnimationState_KnockBack() {}

	void MainAnimation() override;
	void PreEndAnimation() override;

	void PreEnd() override;
};
