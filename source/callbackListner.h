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
class CallbackBase
{
public:
	CallbackBase() {}
	virtual ~CallbackBase() {}

	// �R�[���o�b�N�ďo
	virtual void Execute(Args... argument) = 0;
};

// �h���N���X
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
*	�w�莞�ԑ҂��R�[���o�b�N�N���X
*******************************************************************************/
// ���N���X(template������ƌ^���w�肷��K�v���o�Ă��܂��̂Ŏ󂯎M�Ƃ���)
class Component;

class CallbackInvokeBase
{
public:
	virtual ~CallbackInvokeBase() {}

	// �R�[���o�b�N�ďo�m�F
	virtual bool CheckTime() = 0;
};

// �h���N���X
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
