/*******************************************************************************
*
*	タイトル：	ロゴ表示画面管理スクリプト
*	ファイル：	developLogoManager.cpp
*	作成者：	古本 泰隆
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
	// トランジション情報作成
	Transition::TransitionOption info;
	info.Mode[(int)Transition::TransitionState::Out] = Transition::TransitionPattern::Fade;
	info.TimeLimitSec[(int)Transition::TransitionState::Out] = 1.5f;
	info.Mode[(int)Transition::TransitionState::In] = Transition::TransitionPattern::Fade;
	info.TimeLimitSec[(int)Transition::TransitionState::In] = 1.5f;
	info.Texture = TextureContainer::GetTexture_InName("Black");

	// シーン遷移予約
	Manager::SetScene<Title>(info);

	m_IsNextScene = true;
}
