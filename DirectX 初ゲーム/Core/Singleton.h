#pragma once
#include "Common.h"

template<class T>
class Singleton
{
public:
	static void Create(void)
	{
		if (m_pInstance == nullptr)
			m_pInstance = new T;
	}

	static void Destroy(void)
	{
		SafeDelete(m_pInstance);
	}

	static T* GetInstance(void)
	{
		return m_pInstance;
	}

	virtual ~Singleton(void) {};

protected:
	static T* m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;