/*******************************************************************************
*
*	タイトル：	プレイヤー入力受付スクリプト
*	ファイル：	playerInputListener.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "playerInputListener.h"
#include "playerManager.h"
#include "input.h"
#include "collisionContactRecorder_PlayerGround.h"

void PlayerInputListener::UpdateQueue()
{
	CheckValidTime();
	DeleteElement();
	UpdateCoolTime();

	// 入力を取得してキューに追加
	SetMoveAction();
	SetJumpAction();
	SetAvoidAction();
	SetWeaponDeformationAction();
	if (m_Manager->GetParameter()->m_IsGun)
	{
		//SetShotAction();
	}
	else
	{
		SetRollAction();
	}
}

void PlayerInputListener::CheckValidTime()
{
	for (ActionElement* elem : m_ActionQueue)
	{
		elem->ValidTime -= Time::GetDeltaTimeSec();
		if (elem->ValidTime <= 0.0f)
		{
			DeleteRequest(elem);
		}
	}
}

void PlayerInputListener::DeleteElement()
{
	for (ActionElement* elem : m_DeleteList)
	{
		auto found = find(m_ActionQueue.begin(), m_ActionQueue.end(), elem);
		if (found != m_ActionQueue.end())
		{
			m_ActionQueue.erase(found);
			delete elem;
		}
	}

	m_DeleteList.clear();
}

void PlayerInputListener::UpdateCoolTime()
{
	for (int i = 0; i < (int)ActionId::Length; i++)
	{
		if (m_ActionCoolTime[i])
		{
			m_ActionCoolTime[i].value() -= Time::GetDeltaTimeSec();
			if (m_ActionCoolTime[i].value() <= 0.0f)
			{
				m_ActionCoolTime[i].reset();
			}
		}
	}
}

void PlayerInputListener::DeleteRequest(ActionElement* Elem)
{
	auto found = find(m_DeleteList.begin(), m_DeleteList.end(), Elem);
	if (found == m_DeleteList.end())
	{
		m_DeleteList.push_back(Elem);
	}
}

void PlayerInputListener::SetActionCoolTime(ActionId Id, float Time)
{
	m_ActionCoolTime[(int)Id] = Time;
}

void PlayerInputListener::ResetActionCoolTime(ActionId Id)
{
	m_ActionCoolTime[(int)Id].reset();
}

void PlayerInputListener::ResetActionCoolTimeAll()
{
	for (int i = 0; i < (int)ActionId::Length; i++)
	{
		m_ActionCoolTime[i].reset();
	}
}

void PlayerInputListener::DeleteElementAll()
{
	for (auto elem : m_ActionQueue)
	{
		delete elem;
	}
	m_ActionQueue.clear();
}

void PlayerInputListener::SetMoveAction()
{
	float valueFB = InputMove_Z();
	// 長押しの入力なので先行入力はなし
	ActionElement* elem = DBG_NEW ActionElement(ActionId::Move_Z, valueFB, 0.0f);
	m_ActionQueue.push_back(elem);

	float valueLR = InputMove_X();
	// 長押しの入力なので先行入力はなし
	elem = DBG_NEW ActionElement(ActionId::Move_X, valueLR, 0.0f);
	m_ActionQueue.push_back(elem);
}

void PlayerInputListener::SetJumpAction()
{
	if (InputJump() && m_Manager->GetGroundCollision()->GetIsContact())
	{
		ActionElement* elem = DBG_NEW ActionElement(ActionId::Jump, 1.0f, m_VALID_TIME);
		m_ActionQueue.push_back(elem);
	}
}

void PlayerInputListener::SetAvoidAction()
{
	if (InputAvoid())
	{
		ActionElement* elem = DBG_NEW ActionElement(ActionId::Avoid, 1.0f, m_VALID_TIME);
		m_ActionQueue.push_back(elem);
	}
}

void PlayerInputListener::SetRollAction()
{
	optional<float> valueL = InputRoll_L();
	if (valueL)
	{
		// 長押しの入力なので先行入力はなし
		ActionElement* elem = DBG_NEW ActionElement(ActionId::Roll_Left, valueL.value(), 0.0f);
		m_ActionQueue.push_back(elem);
	}

	optional<float> valueR = InputRoll_R();
	if (valueR)
	{
		// 長押しの入力なので先行入力はなし
		ActionElement* elem = DBG_NEW ActionElement(ActionId::Roll_Right, valueR.value(), 0.0f);
		m_ActionQueue.push_back(elem);
	}
}

void PlayerInputListener::SetWeaponDeformationAction()
{
	if (InputWeaponDeformation())
	{
		ActionElement* elem = DBG_NEW ActionElement(ActionId::WeaponDeformation, 1.0f, m_VALID_TIME);
		m_ActionQueue.push_back(elem);
	}
}
