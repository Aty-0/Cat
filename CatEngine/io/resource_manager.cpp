#include "resource_manager.h"
#include "core/utils/logger.h"
#include <filesystem>

namespace cat::io
{
	resource_manager::resource_manager()
	{

	}

	resource_manager::~resource_manager()
	{

	}

	std::ifstream resource_manager::get_file(const char* type, const char* name, std::vector<const char*> ext)
	{
		const auto PATH_TO_DATA = DATA_NAME + "/" + std::string(type);

		if (!std::filesystem::is_directory(DATA_NAME)
			|| !std::filesystem::exists(DATA_NAME))
		{
			FATAL("resource_manager::get_file Gamedata folder is not exist. We can't continue");
			return std::ifstream();
		}

		if (!std::filesystem::is_directory(PATH_TO_DATA)
			|| !std::filesystem::exists(PATH_TO_DATA))
		{
			try
			{
				if (!std::filesystem::create_directory(PATH_TO_DATA))
				{
					FATAL("resource_manager::get_file Can't create directory...");
					return std::ifstream();
				}
			}
			catch (std::exception ex)
			{
				FATAL("resource_manager::get_file Can't create directory... maybe path or type name is corrupted... | %s", ex.what());
				return std::ifstream();
			}
		}



		for (const auto& cur_ext : ext)
		{			
			const auto fpath = PATH_TO_DATA + "/" + name + "." + cur_ext;

			if (std::filesystem::exists(fpath))
			{
				// File was found 
				std::ifstream file = std::ifstream(fpath, std::ios::binary | std::ios::ate);
				if (!file)
				{
					ERR("resource_manager::get_file Can't open file");
					return std::ifstream(); // Because we are found the file, but we can't open it 
				}

				VERB("resource_manager::get_file File found %s", fpath.c_str());
				return file;
			}
		}

		VERB("resource_manager::get_file File is not found");
		// File is not exist
		return std::ifstream();
	}

}