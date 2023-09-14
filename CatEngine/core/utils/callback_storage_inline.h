#pragma once
#include "core/utils/callback_storage.h"

namespace cat::core::utils
{
	template<typename T>
	callback_storage<T>::callback_storage()
		: m_storage()
	{

	}

	template<typename T>
	callback_storage<T>::callback_storage(const T& callback)
		: m_storage()
	{
		add(callback);
	}

	template<typename T>
	callback_storage<T>::~callback_storage()
	{
		m_storage.clear();
	}

	template<typename T>
	void callback_storage<T>::update(const T& callback)
	{
		remove(callback);
		add(callback);
	}

	template<typename T>
	void callback_storage<T>::add(const T& callback)
	{
		m_storage.push_back(callback);
	}

	template<typename T>
	void callback_storage<T>::remove(const T& callback)
	{
		m_storage.erase(std::remove(m_storage.begin(), m_storage.end(), callback), m_storage.end());
	}

	template<typename T>
	void callback_storage<T>::clear()
	{
		m_storage.clear();
	}

	template<typename T>
	inline void callback_storage<T>::operator()()
	{
		run_all();
	}

	template<typename T>
	void callback_storage<T>::run_all()
	{
		for (const auto& callback : m_storage)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}
}