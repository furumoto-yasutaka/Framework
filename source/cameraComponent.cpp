/*******************************************************************************
*
*	タイトル：	システム系コンポーネント基底クラス
*	ファイル：	cameraComponent.cpp
*	作成者：	古本 泰隆
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
