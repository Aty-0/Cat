#pragma once

// TODO: Get rid of this macros, make universal function 

// Allows assert fails 
//#define CUTE_CAT_ASSERT
#ifdef CUTE_CAT_ASSERT
	#define CAT_ASSERT(a) if(!(a)) ERR("Assertion failed %s\n File:%s\n Line:%d", #a,  __FILE__, __LINE__); 
#else 
	#define CAT_ASSERT(a) if(!(a)) FATAL("Assertion failed %s\n File:%s\n Line:%d", #a, __FILE__, __LINE__); 
#endif 


#define N(X,Y) X##Y
#define N2(X,Y) N(X,Y)

#include <string>
#include <cstdarg> 

namespace cat::core::utils
{	
	template<typename T>
	inline void safeDelete(T*& object)
	{
		if (object != nullptr)
		{
			delete object;
			object = nullptr;
		}
	}

	inline std::string toStr(const char* fmt, ...)
	{
		const auto len = std::size_t { 1024 };
		char buffer[len];
		std::va_list args;
		va_start(args, fmt);
		auto result = std::vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);
		if (result < 0)
			throw std::runtime_error("size of vsnprintf is less then zero...");
		
		buffer[result] = '\0';

		return std::string{ buffer, static_cast<std::size_t>(result) };
	}

	template<typename T>
	[[nodiscard]] inline std::string getClassNameStr(T obj)
	{
		// Get class name by typeid function
		auto class_name = std::string(typeid(obj).name());

		// Remove all namespaces
		auto pos = class_name.find(" ");

		while (pos != std::string::npos)
		{
			class_name = class_name.substr(pos + 1);
			pos = class_name.find(":", pos);
		}
		// FIX ME
		class_name.erase(0, 1);

		// Remove other garbage if we are have x64 pointers
		pos = 0;
		pos = class_name.find(" ", pos);
		if (pos != std::string::npos)
		{
			class_name.erase(pos, class_name.length() - 1);
		}

		return class_name;	
	}

	template<typename T>
	[[nodiscard]] inline std::string getClassNameStr()
	{
		// Get class name by typeid function
		auto class_name = std::string(typeid(T).name());

		// Remove all namespaces
		auto pos = class_name.find(" ");

		while (pos != std::string::npos)
		{
			class_name = class_name.substr(pos + 1);
			pos = class_name.find(":", pos);
		}
		// FIX ME
		class_name.erase(0, 1);

		// Remove other garbage if we are have x64 pointers
		pos = 0;
		pos = class_name.find(" ", pos);
		if (pos != std::string::npos)
		{
			class_name.erase(pos, class_name.length() - 1);
		}

		return class_name;
	}

	[[nodiscard]] inline std::string getFileExtension(const std::string& filename)
	{
		const auto off = filename.find_last_of('.');

		if (off == std::string::npos)
			return std::string();

		return std::string(filename.substr(off + 1));
	}
}