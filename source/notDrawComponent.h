/*******************************************************************************
*
*	�^�C�g���F	�X�N���v�g�n�R���|�[�l���g���N���X
*	�t�@�C���F	notDrawComponent.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "component.h"

class NotDrawComponent : public Component
{
public:
	NotDrawComponent(GameObject* attachObject, int updatePriority = 0);
	virtual ~NotDrawComponent() {}
};
