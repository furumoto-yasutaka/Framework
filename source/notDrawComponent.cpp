/*******************************************************************************
*
*	�^�C�g���F	�X�N���v�g�n�R���|�[�l���g���N���X
*	�t�@�C���F	notDrawComponent.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "notDrawComponent.h"
#include "manager.h"

NotDrawComponent::NotDrawComponent(GameObject* attachObject, int updatePriority)
	: Component(attachObject, updatePriority)
{
	m_DrawGroup = DrawGroup::NotDraw;
}
