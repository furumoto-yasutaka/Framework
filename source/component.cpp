/*******************************************************************************
*
*	�^�C�g���F	�R���|�[�l���g���N���X
*	�t�@�C���F	component.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "component.h"
#include "gameObject.h"

Component::Component(GameObject* attachObject, int updatePriority)
	: m_AttachObject(attachObject),
	m_UpdatePriority(updatePriority)
{
	m_CreateOrderComponents.push_back(this);
}

Component::~Component()
{
	DeleteOrderList();
	ReleaseInvoke();
}

void Component::Destroy()
{
	Uninit();
#ifdef _DEBUG
	UninitDebug();
#endif
	delete this;
}

void Component::CheckInvoke()
{
	m_InvokeList.remove_if(
		[](CallBackInvokeBase* c) { return c->CheckTime(); });
}

void Component::DeleteInvoke(CallBackInvokeBase* instance)
{
	m_InvokeList.remove(instance);
}

/*******************************************************************************
*	�������X�g��ύX���ALateInit���Ăяo��
*******************************************************************************/
void Component::ChangeOrderList()
{
	// ���X�g���ړ�
	m_CreateOrderComponents.remove(this);
	m_UpdatePriorityOrderComponents.insert({ m_UpdatePriority, this });

	LateInit();
}

/*******************************************************************************
*	m_CreateOrderComponents��m_UpdatePriorityOrderComponents����T�����č폜
*******************************************************************************/
void Component::DeleteOrderList()
{
	auto listItr = find(m_CreateOrderComponents.begin(), m_CreateOrderComponents.end(), this);

	if (listItr != m_CreateOrderComponents.end())
	{
		m_CreateOrderComponents.erase(listItr);
	}
	else
	{// m_CreateOrderComponents�Ɍ�����Ȃ�������Am_UpdatePriorityOrderComponents������
		auto range = m_UpdatePriorityOrderComponents.equal_range(m_UpdatePriority);
		auto mapItr = find_if(range.first, range.second,
			[&](pair<int, Component*> pair) { return pair.second == this; });

		if (mapItr != range.second)
		{
			m_UpdatePriorityOrderComponents.erase(mapItr);
		}
	}
}

/*******************************************************************************
*	Invoke�C���X�^���X�����
*******************************************************************************/
void Component::ReleaseInvoke()
{
	m_InvokeList.remove_if([](CallBackInvokeBase* c)
	{
		delete c;
		return true;
	});
}

bool Component::GetIsActive()
{
	return (m_IsActive && m_AttachObject->GetIsActive());
}
