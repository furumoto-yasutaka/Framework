/*******************************************************************************
*
*	�^�C�g���F	���S�\����ʊǗ��X�N���v�g
*	�t�@�C���F	developLogoManager.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "developLogoManager.h"
#include "input.h"
#include "transition.h"
#include "manager.h"
#include "title.h"

void DevelopLogoManager::Init()
{
	//m_InvokeInstance = SetInvoke(&DevelopLogoManager::ChangeTitleScene, m_TimeLimit);
}

void DevelopLogoManager::Update()
{
	if (m_IsNextScene) { return; }

	//if (Input::GetKeyboardTrigger(DIK_SPACE))
	//{
	//	DeleteInvoke(m_InvokeInstance);
	//}
}

void DevelopLogoManager::ChangeTitleScene()
{
	// �g�����W�V�������쐬
	Transition::TransitionOption info;
	info.Mode[(int)Transition::TransitionState::Out] = Transition::TransitionPattern::Fade;
	info.TimeLimitSec[(int)Transition::TransitionState::Out] = 1.5f;
	info.Mode[(int)Transition::TransitionState::In] = Transition::TransitionPattern::Fade;
	info.TimeLimitSec[(int)Transition::TransitionState::In] = 1.5f;
	info.Texture = TextureContainer::GetTexture_InName("Black");

	// �V�[���J�ڗ\��
	Manager::SetScene<Title>(info);

	m_IsNextScene = true;
}
