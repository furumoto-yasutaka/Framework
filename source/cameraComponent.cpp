/*******************************************************************************
*
*	�^�C�g���F	�V�X�e���n�R���|�[�l���g���N���X
*	�t�@�C���F	cameraComponent.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "cameraComponent.h"
#include "manager.h"

CameraComponent::CameraComponent(GameObject* attachObject, int updatePriority)
	: Component(attachObject, updatePriority)
{
	m_DrawGroup = DrawGroup::Camera;

	m_GroupComponents.push_back(this);
}

CameraComponent::~CameraComponent()
{
	m_GroupComponents.remove(this);
}
