#pragma once
#include "core/common.h"

namespace cat::core::utils
{
	using default_callback = std::function<void()>;
	template<typename T = default_callback>
	class CATENGINE_API callback_storage
	{
	public:
		callback_storage();
		explicit callback_storage(const T& callback);
		~callback_storage();

		void update(const T& callback);
		void add(const T& callback);
		void remove(const T& callback);
		void clear();
		void run_all();

		void operator()();
		// TODO: operator += 
	private:
		std::vector<T> m_storage;
	};
}

#include "core/utils/callback_storage_inline.h"