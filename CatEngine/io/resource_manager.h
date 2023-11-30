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

		// read game file by type 
		template<typename data, typename type>
		inline data read(const char* name, std::vector<const char*> ext);
		
		// check file on exist in gamedata 
		template<typename data_type>
		inline bool is_exist(const char* name, std::vector<const char*> ext);

		// get file size in bytes
		template<typename data_type>
		inline std::int64_t size(const char* name, std::vector<const char*> ext);

		void move_files_to_data(std::int32_t count, const char** paths);
	private:
		// TODO: get default file when we are can't find needed
	
		// get path to file 
		// if it not exist you will get "%type_name_folder%/default"		
		std::ifstream get_file(const char* type, const char* name, std::vector<const char*> ext);


		const std::string DATA_NAME = "gamedata";
	};
}

#include "resource_manager_inline.h"