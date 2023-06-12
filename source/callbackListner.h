/*******************************************************************************
*
*	タイトル：	コールバック用クラス
*	ファイル：	callbackListner.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "time.h"

/*******************************************************************************
*	コールバッククラス
*******************************************************************************/
// 基底クラス
// (templateがあると型を指定する必要が
//  出てしまうので受け皿として)
template<typename... Args>
class CallbackBase
{
public:
	CallbackBase() {}
	virtual ~CallbackBase() {}

	// コールバック呼出
	virtual void Execute(Args... argument) = 0;
};

// 派生クラス
template<class T, typename... Args>
class Callback : public CallbackBase<Args...>
{
private:
	T* m_Origin;
	void(T::* m_Callback)(Args...);

public:
	Callback(T* origin, void(T::* callback)(Args...))
		: m_Origin(origin), m_Callback(callback)
	{}
	~Callback() {}

	void Execute(Args... argument) override
	{
		(m_Origin->*m_Callback)(argument...);
	}
};

/*******************************************************************************
*	指定時間待ちコールバッククラス
*******************************************************************************/
// 基底クラス(templateがあると型を指定する必要が出てしまうので受け皿として)
class Component;

class CallbackInvokeBase
{
public:
	virtual ~CallbackInvokeBase() {}

	// コールバック呼出確認
	virtual bool CheckTime() = 0;
};

// 派生クラス
template<class T>
class CallbackInvoke : public CallbackInvokeBase
{
private:
	T* m_Origin;
	void(T::* m_Callback)();
	float m_Time;

public:
	CallbackInvoke(Component* origin, void(T::* callback)(), float time)
		: m_Origin(reinterpret_cast<T*>(origin)),
		m_Callback(callback),
		m_Time(time)
	{}
	~CallbackInvoke() {}

	bool CheckTime() override
	{
		if (m_Time <= 0.0f)
		{
			(m_Origin->*m_Callback)();
			delete this;
			return true;
		}
		else
		{
			m_Time -= Time::GetDeltaTimeSec();
			return false;
		}
	}
};
