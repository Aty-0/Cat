#include "resource_manager.h"
#include "core/utils/logger.h"
#include <filesystem>

namespace cat::io
{
	resource_manager::resource_manager()
	{
		if (!std::filesystem::is_directory(DATA_NAME)
			|| !std::filesystem::exists(DATA_NAME))
		{
			FATAL("Gamedata folder is not exist.");
		}
	}

	resource_manager::~resource_manager()
	{

	}	

	std::ifstream resource_manager::get_file(const char* type, const char* name, std::vector<const char*> ext)
	{
		const auto PATH_TO_DATA = DATA_NAME + "/" + std::string(type);

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

	void resource_manager::move_files_to_data(std::int32_t count, const char** paths)
	{
		const std::initializer_list<std::pair<const char*, const char*>> exts =
		{
			{ "Texture", "png" },
			{ "Texture", "jpg" },
			{ "Texture", "pnm" },
			{ "Texture", "tga" },
			{ "Texture", "bmp" },
			{ "Texture", "hdr" },
			{ "Texture", "pic" },
			{ "Script", "lua" },
			{ "Shader", "glsl" },
		};

		for (std::int32_t i = 0; i < count; i++)
		{
			VERB("resource_manager::move_files_to_data | paths:%s", paths[i]);

			std::string file_name = paths[i];
			auto pos = file_name.rfind("\\");
			file_name.erase(0, pos + 2);

			auto ext = core::utils::get_file_extension(file_name);
			for (auto cur_ext = exts.begin(); cur_ext != exts.end(); cur_ext++)
			{
				if (cur_ext->second == ext)
				{
					VERB("Adding to data...");

					const auto PATH_TO_DATA = DATA_NAME + "/" + std::string(cur_ext->first);
					const auto fpath = PATH_TO_DATA + "/" + file_name + "." + cur_ext->second;
					try
					{
						std::filesystem::copy_file(paths[i], fpath);
					}
					catch (std::filesystem::filesystem_error& e)
					{
						ERR("Could not copy %s %s", paths[i], e.what());
					}

					break;
				}
			}
		}
	}

}