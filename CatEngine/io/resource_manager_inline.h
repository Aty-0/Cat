#pragma once
#include "io/resource_manager.h"
#include "core/utils/logger.h"
#include "core/utils/utilsbase.h" // FIXME: Need to check common 

namespace cat::io
{
	template<typename data, typename type>
	inline data resource_manager::get(const char* name, std::vector<const char*> ext)
	{
		data data_ptr = (data)m_storage[name];

		if (data_ptr == nullptr)
		{
			data_ptr = read<data, type>(name, ext);
			
			if (data_ptr == nullptr)
				return nullptr;

			m_storage[name] = (void*)data_ptr;

			return data_ptr;
		}


		return data_ptr;
	}


	template<typename data, typename type>
	inline data resource_manager::read(const char* name, std::vector<const char*> ext)
	{
		const auto type_name = core::utils::getClassNameStr<type>();
		
		VERB("resource_manager::read %s %s", type_name.c_str(), name);
		std::ifstream file = getFile(type_name.c_str(), name, ext);
		if (!file.is_open())
			return nullptr;
		
		std::int64_t size = file.tellg();
		file.seekg(0, std::ios::beg);
		
		auto buffer = new char[size + 1];
		buffer[size] = '\0';

		if (!file.read(buffer, size))
		{
			ERR("resource_manager::read Can't read file data.");
			return nullptr;
		}

		file.close();

		return reinterpret_cast<data>(buffer);
	}

	template<typename data_type>
	inline bool resource_manager::isExist(const char* name, std::vector<const char*> ext)
	{
		const auto type_name = core::utils::getClassNameStr<data_type>();
		VERB("resource_manager::is_exist %s %s", type_name.c_str(), name);
		auto file = get_file(type_name.c_str(), name, ext);
		auto is_open = file.is_open();
		file.close();

		return is_open;
	}

	template<typename data_type>
	inline std::int64_t resource_manager::size(const char* name, std::vector<const char*> ext)
	{
		const auto type_name = core::utils::getClassNameStr<data_type>();
		VERB("resource_manager::size %s %s", type_name.c_str(), name);

		auto file = getFile(type_name.c_str(), name, ext);
		
		if (file.is_open())
		{
			auto size = file.tellg();
			file.close();
			return size;
		}
		else
		{ 
			file.close();
			return 0;
		}
	}
}