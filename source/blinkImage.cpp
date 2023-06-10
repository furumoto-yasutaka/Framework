/*******************************************************************************
*
*	�^�C�g���F�@�摜�_�ŃX�N���v�g
*	�t�@�C���F	blinkImage.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "blinkImage.h"
#include "time.h"
#include "plate2dRenderer.h"
#include "gameObject.h"

void BlinkImage::Init(BlinkMode mode, float visibleTimeSec, float inVisibleTimeSec,
					float changeVisibleTimeSec, float changeInVisibleTimeSec)
{
	m_Mode = mode;
	m_State = BlinkState::Visible;
	m_TimeCountSec = visibleTimeSec;
	m_VisibleTimeSec = visibleTimeSec;
	m_InVisibleTimeSec = inVisibleTimeSec;
	m_ChangeVisibleTimeSec = changeVisibleTimeSec;
	m_ChangeInVisibleTimeSec = changeInVisibleTimeSec;
}

void BlinkImage::Update()
{
	m_TimeCountSec -= Time::GetDeltaTimeSec();

	if (m_TimeCountSec > 0.0f)
	{
		ChangeAlpha();
	}
	else
	{
		ChangeState();
	}
}

/*******************************************************************************
*	�����x�ύX
*******************************************************************************/
void BlinkImage::ChangeAlpha()
{
	switch (m_Mode)
	{
	case BlinkMode::Fade:
		ChangeAlpha_Fade();
		break;
	default: return;
	}
}

/*******************************************************************************
*	�����x�ύX(�t�F�[�h���[�h)
*******************************************************************************/
void BlinkImage::ChangeAlpha_Fade()
{
	Plate2DRenderer* plate2dRenderer = m_AttachObject->GetComponent<Plate2DRenderer>();

	switch (m_State)
	{
	case BlinkState::ChangeInVisible:
		plate2dRenderer->SetTexAlphaColor(m_TimeCountSec / m_ChangeInVisibleTimeSec);
		break;
	case BlinkState::ChangeVisible:
		plate2dRenderer->SetTexAlphaColor(1.0f - (m_TimeCountSec / m_ChangeVisibleTimeSec));
		break;
	default: return;
	}
}

/*******************************************************************************
*	�_�ŏ󋵕ύX
*******************************************************************************/
void BlinkImage::ChangeState()
{
	switch (m_Mode)
	{
	case BlinkMode::None:
		ChangeState_None();
		break;
	case BlinkMode::Fade:
		ChangeState_Fade();
		break;
	default: return;
	}
}

/*******************************************************************************
*	�_�ŏ󋵕ύX(���ݒ胂�[�h)
*******************************************************************************/
void BlinkImage::ChangeState_None()
{
	Plate2DRenderer* plate2dRenderer = m_AttachObject->GetComponent<Plate2DRenderer>();

	switch (m_State)
	{
	case BlinkState::Visible:
		m_State = BlinkState::InVisible;
		m_TimeCountSec = m_InVisibleTimeSec;
		plate2dRenderer->SetTexAlphaColor(0.0f);
		break;
	case BlinkState::InVisible:
		m_State = BlinkState::Visible;
		m_TimeCountSec = m_VisibleTimeSec;
		plate2dRenderer->SetTexAlphaColor(1.0f);
		break;
	default: return;
	}
}

/*******************************************************************************
*	�_�ŏ󋵕ύX(�t�F�[�h���[�h)
*******************************************************************************/
void BlinkImage::ChangeState_Fade()
{
	Plate2DRenderer* plate2dRenderer = m_AttachObject->GetComponent<Plate2DRenderer>();

	switch (m_State)
	{
	case BlinkState::Visible:
		m_State = BlinkState::ChangeInVisible;
		m_TimeCountSec = m_ChangeInVisibleTimeSec;
		break;
	case BlinkState::ChangeInVisible:
		m_State = BlinkState::InVisible;
		m_TimeCountSec = m_InVisibleTimeSec;
		plate2dRenderer->SetTexAlphaColor(0.0f);
		break;
	case BlinkState::InVisible:
		m_State = BlinkState::ChangeVisible;
		m_TimeCountSec = m_ChangeVisibleTimeSec;
		break;
	case BlinkState::ChangeVisible:
		m_State = BlinkState::Visible;
		m_TimeCountSec = m_VisibleTimeSec;
		plate2dRenderer->SetTexAlphaColor(1.0f);
		break;
	default: return;
	}
}
