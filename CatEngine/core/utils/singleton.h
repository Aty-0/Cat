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
		
		[[nodiscard]] inline static decltype(auto) getInstance() 
		{
			static auto instance = std::make_shared<T>();
			return instance.get();
		}

		singleton& operator=(const singleton&) = delete;
		singleton& operator=(singleton&&) = delete;

	};
}