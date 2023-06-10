/*******************************************************************************
*
*	�^�C�g���F	3D�`��R���|�[�l���g���N���X
*	�t�@�C���F	draw3dComponent.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "draw3dComponent.h"
#include "manager.h"

Draw3DComponent::Draw3DComponent(GameObject* attachObject, int updatePriority)
	: Component(attachObject, updatePriority)
{
	m_DrawGroup = DrawGroup::Draw3D;

	m_GroupComponents.push_back(this);
}

Draw3DComponent::~Draw3DComponent()
{
	m_GroupComponents.remove(this);
}
