#pragma once
#include "core/common.h"
#include <fstream>

namespace cat::io
{
	class CATENGINE_API resource_manager : public core::utils::singleton<resource_manager>
	{
	public:
		resource_manager();
		~resource_manager();

		// load or get some data 
		template<typename data, typename type>
		inline data get(const char* name, std::vector<const char*> ext);

		// read game file by type 
		template<typename data, typename type>
		inline data read(const char* name, std::vector<const char*> ext);
		
		// check file on exist in gamedata 
		template<typename data_type>
		inline bool isExist(const char* name, std::vector<const char*> ext);

		// get file size in bytes
		template<typename data_type>
		inline std::size_t size(const char* name, std::vector<const char*> ext);

		void moveFilesToData(std::int32_t count, const char** paths);
	private:
		// TODO: get default file when we are can't find needed
	
		// get path to file 
		// if it not exist you will get "%type_name_folder%/default"		
		std::ifstream getFile(const char* type, const char* name, std::vector<const char*> ext);

	public:
		static const std::string DATA_NAME;

		std::unordered_map<const char*, void*> m_storage;
	};
}

#include "resource_manager_inline.h"