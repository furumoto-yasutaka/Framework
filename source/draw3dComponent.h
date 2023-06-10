/*******************************************************************************
*
*	�^�C�g���F	3D�`��R���|�[�l���g���N���X
*	�t�@�C���F	draw3dComponent.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "component.h"

class Draw3DComponent : public Component
{
protected:
	// �`�揈���p
	static inline list<Draw3DComponent*> m_GroupComponents;

public:
	Draw3DComponent(GameObject* attachObject, int updatePriority = 0);
	virtual ~Draw3DComponent();

	virtual void Draw3d() = 0;

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static inline list<Draw3DComponent*> GetGroupComponents()
	{
		return m_GroupComponents;
	}
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawDebug3d() {}
#endif
};
