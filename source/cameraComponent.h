/*******************************************************************************
*
*	�^�C�g���F	�V�X�e���n�R���|�[�l���g���N���X
*	�t�@�C���F	cameraComponent.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "component.h"

class CameraComponent : public Component
{
protected:
	// �`�揈���p
	static inline list<CameraComponent*> m_GroupComponents;

public:
	CameraComponent(GameObject* attachObject, int updatePriority = 0);
	virtual ~CameraComponent();

	virtual void DrawView() = 0;

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static inline list<CameraComponent*> GetGroupComponents()
	{
		return m_GroupComponents;
	}
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawDebug3d() {}
#endif
};
