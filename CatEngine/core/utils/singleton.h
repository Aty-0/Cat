#pragma once
#include <memory>

namespace cat::core::utils
{
	template<typename T>
	class singleton
	{
	public:
		singleton() { }
		singleton(const singleton&) = delete;
		singleton(singleton&&) = delete;
		~singleton() { /* we can debug it if needed */ }
		singleton& operator=(const singleton&) = delete;
		singleton& operator=(singleton&&) = delete;

		[[nodiscard]] inline static T* get_instance()
		{
			static auto instance = std::make_shared<T>();
			return instance.get();
		}

	};
}