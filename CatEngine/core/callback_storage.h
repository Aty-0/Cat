#pragma once
#include "core/common.h"

namespace cat::core
{
	class callback_storage
	{
	public:
		using default_callback = std::function<void()>;

		callback_storage();

		explicit callback_storage(default_callback callback);
		~callback_storage();

		void update(default_callback callback);
		void add(default_callback callback);
		void remove(default_callback callback);
		void clear();
		void run_all();

		void operator()();
		void operator+=(default_callback func);

	private:
		std::vector<default_callback> m_storage;
	};

	inline void callback_storage::operator+=(default_callback func)
	{
		add(func);
	}

	inline void callback_storage::operator()()
	{
		run_all();
	}

	inline callback_storage::callback_storage()
		: m_storage()
	{

	}

	inline callback_storage::callback_storage(default_callback callback)
		: m_storage()
	{
		add(callback);
	}

	inline callback_storage::~callback_storage()
	{
		m_storage.clear();
	}

	inline void callback_storage::update(default_callback callback)
	{
		remove(callback);
		add(callback);
	}

	inline void callback_storage::add(default_callback callback)
	{
		m_storage.push_back(callback);
	}

	inline void callback_storage::remove(default_callback callback)
	{
		// FIX ME: 
		//auto removed = std::remove(m_storage.begin(), m_storage.end(), callback);
		//m_storage.erase(removed, m_storage.end());
	}

	inline void callback_storage::clear()
	{
		m_storage.clear();
	}

	inline void callback_storage::run_all()
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
