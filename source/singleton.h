/*******************************************************************************
*
*	�^�C�g���F	�V���O���g�����N���X�E�Ǘ��N���X
*	�t�@�C���F	singleton.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

template <class T>
class Singleton
{
private:
	static inline T* m_Instance = NULL;

public:
	virtual ~Singleton()
	{
		DestroyInstance();
	}

	void* operator new(size_t n)
	{
		void* p = NULL;
		if (!m_Instance)
		{
			p = malloc(n);
		}
		
		return p ? p : throw bad_alloc();
	}

	static inline T& GetInstance()
	{
		if (m_Instance)
		{
			return *m_Instance;
		}
		else
		{
			assert(m_Instance);
			return NULL;
		}
	}

	static inline bool CheckInstance()
	{
		return m_Instance;
	}
		

private:
	static inline void DestroyInstance()
	{
		m_Instance = NULL;
	}
};
