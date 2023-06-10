/*******************************************************************************
*
*	�^�C�g���F	�e�N�X�`���A�j���[�V�����R���|�[�l���g
*	�t�@�C���F	textureAnimation.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "textureAnimation.h"
#include "gameObject.h"

void TextureAnimation::Init(Plate2DRenderer* connectComponent, AnimationMode mode, int animationInterval)
{
	// Plate2DRenderer�Ɛڑ�
	connectComponent->Connect(this, m_TexCoordBegin, m_TexCoordDistance, m_WidthDiv, m_HeightDiv);

	// ������
	m_Mode = mode;
	m_FrameCount = 0;
	m_AnimationInterval = animationInterval;
	m_TexCoordDistance->x = 1.0f / m_WidthDiv;
	m_TexCoordDistance->y = 1.0f / m_HeightDiv;
}

void TextureAnimation::Init(Plate3DRenderer* connectComponent, AnimationMode mode, int animationInterval)
{
	// Plate3DRenderer�Ɛڑ�
	connectComponent->Connect(this, m_TexCoordBegin, m_TexCoordDistance, m_WidthDiv, m_HeightDiv);

	// ������
	m_Mode = mode;
	m_FrameCount = 0;
	m_AnimationInterval = animationInterval;
	m_TexCoordDistance->x = 1.0f / m_WidthDiv;
	m_TexCoordDistance->y = 1.0f / m_HeightDiv;
}

void TextureAnimation::Update()
{
	// �e�N�X�`�����W�v�Z
	int progres = m_FrameCount / m_AnimationInterval;
	m_TexCoordBegin->x = progres % m_WidthDiv * m_TexCoordDistance->x;
	m_TexCoordBegin->y = progres / m_WidthDiv * m_TexCoordDistance->y;

	// �A�j���[�V�����̏I�[�ɒB������
	if (m_FrameCount >= m_WidthDiv * m_HeightDiv * m_AnimationInterval)
	{
		switch (m_Mode)
		{
		case AnimationMode::Loop:
			m_FrameCount = 0;
			break;
		case AnimationMode::Once_ObjectDestroy:
			m_AttachObject->SetDestroy();
			break;
		case AnimationMode::Once_ObjectDisable:
			m_FrameCount = 0;
			m_AttachObject->SetIsActive(false);
			break;
		default: return;
		}
	}

	// �J�E���g�i�s
	m_FrameCount++;
}
