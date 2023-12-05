#include "filesystem_tree_ui.h"
#include "graphics/renderer.h"
#include "io/resource_manager.h"
#include <filesystem>
#ifdef _WIN64
	#include <windows.h>
#endif
namespace cat::core::editor
{
	void filesystem_tree_ui::render()
	{
		ImGui::Begin("filesystem");
		
		
		const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
		static const auto rootName = io::resource_manager::DATA_NAME.c_str();

		const std::initializer_list<const char*> types =
		{
			"Texture",
			"Script",
			"Shader"
		};


		if (ImGui::TreeNodeEx(rootName, flags))
		{
			for (auto it = types.begin(); it != types.end(); it++)
			{
				const char* name = *it;
				if (ImGui::TreeNode(name))
				{
					const auto dir = std::string(rootName) + "\\" + name;
					std::int32_t count = 0;

					// TODO: Subfolders
					for (auto const& dir_entry : 
						std::filesystem::directory_iterator{ dir })
					{						
						auto file = dir_entry.path().filename().string();					
						ImGui::PushID(count);
						
						if (ImGui::Selectable(file.c_str()))
						{
#ifdef _WIN64
							const auto __path__ = dir_entry.path().string();
							ShellExecute(NULL, "open", __path__.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
						}
						count++;
						ImGui::PopID();
					}

					ImGui::TreePop();  
				}
			}
			ImGui::TreePop();  
		}

		ImGui::End();

	}
}