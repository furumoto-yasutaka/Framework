/*******************************************************************************
*
*	�^�C�g���F	�摜�_�ŃX�N���v�g
*	�t�@�C���F	blinkImage.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class BlinkImage : public NotDrawComponent
{
public:
	// �_�ŕ��@
	enum class BlinkMode
	{
		None = 0,
		Fade,
	};
	// �_�ł̐i�s��
	enum class BlinkState
	{
		Visible = 0,
		ChangeInVisible,
		InVisible,
		ChangeVisible,
	};

private:
	BlinkMode	m_Mode = BlinkMode::None;			// �_�ŕ��@
	BlinkState	m_State = BlinkState::Visible;		// �_�ł̐i�s��
	float		m_TimeCountSec = 0.0f;				// ���Ԍv��(�b)
	float		m_VisibleTimeSec = 0.0f;			// �\���̌p������
	float		m_InVisibleTimeSec = 0.0f;			// ��\���̌p������
	
	float		m_ChangeVisibleTimeSec = 0.0f;		// �\���ɂ����鎞��
	float		m_ChangeInVisibleTimeSec = 0.0f;	// ��\���ɂ����鎞��

public:
	BlinkImage(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~BlinkImage() {}

	void Init(BlinkMode mode, float visibleTimeSec, float inVisibleTimeSec,
			float changeVisibleTimeSec = 0.0f, float changeInVisibleTimeSec = 0.0f);
	void Uninit() override {}
	void Update() override;

private:
	// �����x�ύX
	void ChangeAlpha();
	// �����x�ύX(�t�F�[�h���[�h)
	void ChangeAlpha_Fade();

	// �_�ŏ󋵕ύX
	void ChangeState();
	// �_�ŏ󋵕ύX(���ݒ胂�[�h)
	void ChangeState_None();
	// �_�ŏ󋵕ύX(�t�F�[�h���[�h)
	void ChangeState_Fade();
};
