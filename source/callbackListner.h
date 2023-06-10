/*******************************************************************************
*
*	�^�C�g���F	�R�[���o�b�N�p�N���X
*	�t�@�C���F	callbackListner.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "time.h"

/*******************************************************************************
*	�R�[���o�b�N�N���X
*******************************************************************************/
// ���N���X
// (template������ƌ^���w�肷��K�v��
//  �o�Ă��܂��̂Ŏ󂯎M�Ƃ���)
template<typename... Args>
class CallBackBase
{
public:
	CallBackBase() {}
	virtual ~CallBackBase() {}

	// �R�[���o�b�N�ďo
	virtual void Execute(Args... argument) = 0;
};

// �h���N���X
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
*	�w�莞�ԑ҂��R�[���o�b�N�N���X
*******************************************************************************/
// ���N���X(template������ƌ^���w�肷��K�v���o�Ă��܂��̂Ŏ󂯎M�Ƃ���)
class Component;

class CallBackInvokeBase
{
public:
	virtual ~CallBackInvokeBase() {}

	// �R�[���o�b�N�ďo�m�F
	virtual bool CheckTime() = 0;
};

// �h���N���X
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
