/*******************************************************************************
*
*	タイトル：	コンポーネント基底クラス
*	ファイル：	component.h
*	作成者：	古本 泰隆
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
	// コンポーネントの分類
	enum class DrawGroup
	{
		Camera = 0,	// カメラ
		Draw3D,		// 3D描画を伴う
		Draw2D,		// 2D描画を伴う
		NotDraw,	// 描画を伴わない
		Length,
	};

protected:
	// 生成直後のコンポーネント(LateInit未呼び出し)
	static inline list<Component*> m_CreateOrderComponents;
	// 更新処理用(LateInit呼び出し済)
	static inline multimap<int, Component*> m_UpdatePriorityOrderComponents;

	GameObject* m_AttachObject;						// アタッチ先オブジェクト
	int			m_UpdatePriority;					// 更新処理優先度
	DrawGroup	m_DrawGroup = DrawGroup::Camera;	// 描画処理グループ
	bool		m_IsActive = true;					// アクティブフラグ
	bool		m_IsDestroy = false;				// 削除フラグ

	list<CallBackInvokeBase*> m_InvokeList;			// 指定時間待ちコールバックリスト

public:
	Component(GameObject* attachObject, int updatePriority);
	virtual ~Component();

	virtual void LateInit() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() {}

	// 自身の削除を予約
	virtual void SetDestroy() { m_IsDestroy = true; }
	// 削除処理
	void Destroy();
	// Invokeリストの呼び出し確認
	void CheckInvoke();
	// Invokeリストの呼び出し確認
	void DeleteInvoke(CallBackInvokeBase* instance);
	// 所属リストを変更し、LateInitを呼び出す
	void ChangeOrderList();

protected:
	// m_CreateOrderComponentsとm_UpdatePriorityOrderComponentsから探索して削除
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
	//　↓↓　アクセサ　↓↓　//
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
