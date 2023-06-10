/*******************************************************************************
*
*	�^�C�g���F	�R���|�[�l���g���N���X
*	�t�@�C���F	component.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "callbackListner.h"

#include <list>
#include <map>

#ifdef _DEBUG
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#endif

class GameObject;

class Component
{
public:
	// �R���|�[�l���g�̕���
	enum class DrawGroup
	{
		Camera = 0,	// �J����
		Draw3D,		// 3D�`��𔺂�
		Draw2D,		// 2D�`��𔺂�
		NotDraw,	// �`��𔺂�Ȃ�
		Length,
	};

protected:
	// ��������̃R���|�[�l���g(LateInit���Ăяo��)
	static inline list<Component*> m_CreateOrderComponents;
	// �X�V�����p(LateInit�Ăяo����)
	static inline multimap<int, Component*> m_UpdatePriorityOrderComponents;

	GameObject* m_AttachObject;						// �A�^�b�`��I�u�W�F�N�g
	int			m_UpdatePriority;					// �X�V�����D��x
	DrawGroup	m_DrawGroup = DrawGroup::Camera;	// �`�揈���O���[�v
	bool		m_IsActive = true;					// �A�N�e�B�u�t���O
	bool		m_IsDestroy = false;				// �폜�t���O

	list<CallBackInvokeBase*> m_InvokeList;			// �w�莞�ԑ҂��R�[���o�b�N���X�g

public:
	Component(GameObject* attachObject, int updatePriority);
	virtual ~Component();

	virtual void LateInit() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() {}

	// ���g�̍폜��\��
	virtual void SetDestroy() { m_IsDestroy = true; }
	// �폜����
	void Destroy();
	// Invoke���X�g�̌Ăяo���m�F
	void CheckInvoke();
	// Invoke���X�g�̌Ăяo���m�F
	void DeleteInvoke(CallBackInvokeBase* instance);
	// �������X�g��ύX���ALateInit���Ăяo��
	void ChangeOrderList();

protected:
	// m_CreateOrderComponents��m_UpdatePriorityOrderComponents����T�����č폜
	void DeleteOrderList();
	void ReleaseInvoke();

	template<class T>
	CallBackInvokeBase* SetInvoke(void(T::* function)(), float time)
	{
		CallBackInvokeBase* c = DBG_NEW CallBackInvoke(this, function, time);
		m_InvokeList.push_back(c);
		return c;
	}

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static inline list<Component*> GetCreateOrderComponents()
	{
		return m_CreateOrderComponents;
	}
	static inline multimap<int, Component*> GetUpdatePriorityOrderComponents()
	{
		return m_UpdatePriorityOrderComponents;
	}

	GameObject* GetAttachObject() { return m_AttachObject; }

	int GetUpdatePriority() { return m_UpdatePriority; }

	DrawGroup GetDrawGroup() { return m_DrawGroup; }

	bool GetIsActive();
	bool GetIsActiveSelf() { return m_IsActive; }
	void SetIsActive(bool value) { m_IsActive = value; }

	bool GetIsDestroy() { return m_IsDestroy; }
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawInspector() {}
	virtual void InitDebug() {}
	virtual void UninitDebug() {}
	virtual void UpdateDebug() {}
#endif
};
