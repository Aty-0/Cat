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

namespace cat::core::utils
{	
	template<typename T>
	inline void safe_delete(T*& object)
	{
		if (object != nullptr)
		{
			delete object;
			object = nullptr;
		}
	}

	template<typename T>
	[[nodiscard]] inline std::string get_class_name_string(T obj)
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
	[[nodiscard]] inline std::string get_class_name_string()
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

	[[nodiscard]] inline std::string get_file_extension(const std::string& filename)
	{
		const auto off = filename.find_last_of('.');

		if (off == std::string::npos)
			return std::string();

		return std::string(filename.substr(off + 1));
	}
}