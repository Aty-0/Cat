#pragma once
#include "core/common.h"

namespace cat::core
{
	class callback_storage
	{
	public:
		using default_callback = std::function<void()>;

		callback_storage();

		explicit callback_storage(const default_callback& callback);
		~callback_storage() 
		{
			m_storage.clear();
			m_storage.shrink_to_fit();
		}

		void update(default_callback callback);
		void add(default_callback callback);
		void remove(default_callback callback);
		void clear();
		void runAll();

		void operator()();
		void operator+=(default_callback func);

		[[nodiscard]] std::size_t size() const;
	private:
		std::vector<default_callback> m_storage;
	};

	inline void callback_storage::operator+=(default_callback func)
	{
		add(func);
	}

	inline void callback_storage::operator()()
	{
		runAll();
	}
}
