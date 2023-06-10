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
class CallBackBase
{
public:
	CallBackBase() {}
	virtual ~CallBackBase() {}

	// コールバック呼出
	virtual void Execute(Args... argument) = 0;
};

// 派生クラス
template<class T, typename... Args>
class CallBack : public CallBackBase<Args...>
{
private:
	T* m_Origin;
	void(T::* m_CallBack)(Args...);

public:
	CallBack(T* origin, void(T::* callBack)(Args...))
		: m_Origin(origin), m_CallBack(callBack)
	{}
	~CallBack() {}

	void Execute(Args... argument) override
	{
		(m_Origin->*m_CallBack)(argument...);
	}
};

/*******************************************************************************
*	指定時間待ちコールバッククラス
*******************************************************************************/
// 基底クラス(templateがあると型を指定する必要が出てしまうので受け皿として)
class Component;

class CallBackInvokeBase
{
public:
	virtual ~CallBackInvokeBase() {}

	// コールバック呼出確認
	virtual bool CheckTime() = 0;
};

// 派生クラス
template<class T>
class CallBackInvoke : public CallBackInvokeBase
{
private:
	T* m_Origin;
	void(T::* m_CallBack)();
	float m_Time;

public:
	CallBackInvoke(Component* origin, void(T::* callBack)(), float time)
		: m_Origin(reinterpret_cast<T*>(origin)),
		m_CallBack(callBack),
		m_Time(time)
	{}
	~CallBackInvoke() {}

	bool CheckTime() override
	{
		if (m_Time <= 0.0f)
		{
			(m_Origin->*m_CallBack)();
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
