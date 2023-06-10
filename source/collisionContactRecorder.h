/*******************************************************************************
*
*	タイトル：	接触オブジェクト管理コンポーネント
*	ファイル：	collisionContactRecorder.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"
#include "gameObject.h"
#include "collider3d.h"

template<class T>
class CollisionContactRecorder : public NotDrawComponent
{
protected:
	T* m_Collider = NULL;

	int m_TargetTag = 0;
	bool m_IsContact = false;
	list<GameObject*> m_OnContactList;

	bool m_IsMatchTag = false;
	bool m_IsPush = false;
	bool m_IsRemove = false;

	bool m_IsInheritance = false;

public:
	CollisionContactRecorder<T>(GameObject* attachObject)
		: NotDrawComponent(attachObject),
		m_IsInheritance(true)
	{}
	virtual ~CollisionContactRecorder<T>() {}

	virtual void Init(int targetTag)
	{
		m_TargetTag = targetTag;
	}
	virtual void LateInit() override
	{
		m_Collider = m_AttachObject->GetComponent<T>();
		if (m_Collider)
		{
			m_Collider->SetOnContactEnter(this, &CollisionContactRecorder<T>::ContactEnter);
			m_Collider->SetOnContactExit(this, &CollisionContactRecorder<T>::ContactExit);
		}
	}
	void Uninit() override {}
	void Update() override {}

	// 衝突したオブジェクトを記録(コールバック)
	virtual void ContactEnter(Collider3D* collider)
	{
		m_IsMatchTag = collider->GetAttachObject()->GetTag() & m_TargetTag;
		if (m_IsMatchTag)
		{
			auto itr = find(m_OnContactList.begin(), m_OnContactList.end(), collider->GetAttachObject());
			m_IsPush = itr == m_OnContactList.end();
			if (m_IsPush)
			{
				m_OnContactList.push_back(collider->GetAttachObject());
				if (m_OnContactList.size() > 0)
				{
					m_IsContact = true;
				}
			}
		}
		else
		{
			m_IsPush = false;
		}
	}
	// 離れたオブジェクトを記録(コールバック)
	virtual void ContactExit(Collider3D* collider)
	{
		m_IsMatchTag = collider->GetAttachObject()->GetTag() & m_TargetTag;
		if (m_IsMatchTag)
		{
			auto itr = find(m_OnContactList.begin(), m_OnContactList.end(), collider->GetAttachObject());
			m_IsRemove = itr != m_OnContactList.end();
			if (m_IsRemove)
			{
				m_OnContactList.erase(itr);
				if (m_OnContactList.size() == 0)
				{
					m_IsContact = false;
				}
			}
		}
		else
		{
			m_IsRemove = false;
		}
	}

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	bool GetIsContact() { return m_IsContact; }
	list<GameObject*> GetOnContactList() { return m_OnContactList; }
	/////////////////////////////
};
