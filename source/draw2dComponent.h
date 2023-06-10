/*******************************************************************************
*
*	�^�C�g���F	2D�`��R���|�[�l���g���N���X
*	�t�@�C���F	draw2dComponent.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "component.h"

class Draw2DComponent : public Component
{
protected:
	// �`�揈���p
	static inline multimap<int, Draw2DComponent*> m_DrawPriorityOrderComponents;

	int m_DrawPriority;		// �`��D��x

public:
	Draw2DComponent(GameObject* attachObject, int updatePriority = 0, int drawPriority = 0);
	virtual ~Draw2DComponent();

	virtual void Draw2d() = 0;

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static inline multimap<int, Draw2DComponent*> GetDrawPriorityOrderComponents()
	{
		return m_DrawPriorityOrderComponents;
	}

	int GetDrawPriority() { return m_DrawPriority; }
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawDebug2d() {}
#endif
};
