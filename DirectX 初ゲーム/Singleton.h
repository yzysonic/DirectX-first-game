#pragma once

template<class T>
class Singleton
{
public:
	static void Create(void);
	static void Destroy(void);
	static T* GetInstance(void);

protected:
	static T* m_pInstance;
};


template<class T>
T* Singleton<T>::m_pInstance = nullptr;

template<class T>
inline void Singleton<T>::Create()
{
	if (m_pInstance == nullptr)
		m_pInstance = new T;
}

template<class T>
inline void Singleton<T>::Destroy()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

template<class T>
T * Singleton<T>::GetInstance()
{
	return m_pInstance;
}
