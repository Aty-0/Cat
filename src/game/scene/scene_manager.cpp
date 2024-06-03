#include "scene_manager.h"
#include "game/components/camera.h"
#include "graphics/renderer.h"

#include "io/resource_manager.h"

// FIXME: Remove
#include "core/input_manager.h"

namespace cat::game::scene
{
	scene_manager::scene_manager() :
		m_scene(nullptr),
		m_selected(nullptr)
	{

	}

	scene_manager::~scene_manager()
	{
		m_scene->destroy();
		m_scene.reset();
	}

	void scene_manager::create()
	{
		INFO("[SceneManager] Create new scene.");

		m_scene = std::make_shared<game::scene::scene>();

		createGameObject<game::game_object>(game::components::camera::EngineCameraName, CAT_ENGINE_GAMEOBJECT_TYPE, -1)->createComponent<game::components::camera>();
		// TODO: Callbacks

		graphics::renderer::onImGuiRender.add(std::bind(&scene_manager::drawEditorSceneInspector, this));

		// TODO: Remove
		const auto input = core::input_manager::getInstance();
		input->addListener(core::input_key_code::KEYBOARD_F10, core::input_key_state::Press, core::input_device::Keyboard, std::bind(&scene_manager::save, this));
		input->addListener(core::input_key_code::KEYBOARD_F9, core::input_key_state::Press, core::input_device::Keyboard, std::bind([this]() {
			load(m_scene->getName());
		}));
	}

	void scene_manager::drawEditorSceneInspector()
	{
		ImGui::Begin("Scene");
		ImGui::Text("GameObject's:");

		static game::game_object* prev = nullptr;
		
		ImGui::Dummy(ImVec2(0.0f, 20.0f));
		ImGui::Separator();
		
		const auto rootSceneName = m_scene->m_name.c_str(); 
		if (ImGui::TreeNodeEx(rootSceneName, 
			ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const auto& object : m_scene->getStorage())
			{
				const auto gameObjectName = object.second->m_name.c_str();
				if (ImGui::TreeNode(gameObjectName))
				{
					if (ImGui::Selectable(gameObjectName, object.second->m_select))
					{
						object.second->m_select = !object.second->m_select;
						m_selected = object.second.get();
						// TODO: Show game object childrens 
					}	

					if (prev && prev != m_selected)
						prev->m_select = false;


					prev = m_selected;
					
					ImGui::TreePop();  
				}   				
			}
			
   			ImGui::TreePop();  
		}

		ImGui::End();


		if (m_selected == nullptr)
			return;
		
		ImGui::Begin("ObjectInspector");
		{		
			
			if (m_selected->m_select)
			{
				ImGui::Text("GameObject propeties:");
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				auto name = (char*)m_selected->m_name.c_str();
				if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name)))
				{
					m_selected->m_name = name;
				}

				ImGui::Text("uuid: %s", m_selected->m_uuid.getIDStr().c_str());
				ImGui::Checkbox("visible", &m_selected->m_visible);
				ImGui::Checkbox("enabled", &m_selected->m_enabled);

				ImGui::Dummy(ImVec2(0.0f, 20.0f));
				ImGui::Separator();

				ImGui::Text("Components:");
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				ImGui::Separator();

				for (const auto& component : m_selected->getComponents())
				{
					if (ImGui::Selectable(component.second->m_name.c_str(),
						component.second->m_select))
					{
						component.second->m_select = !component.second->m_select;
					}

					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					ImGui::Separator();

					if (component.second->m_select)
					{
						const auto callback = component.second->onEditGuiDraw;
						if (callback.size() == 0)
						{
							ImGui::Text("None");
							continue;
						}

						component.second->onEditGuiDraw();
						ImGui::Separator();
					}

				}
			}

			ImGui::End();
		}
	}

	bool scene_manager::load(const std::string& name)
	{
		INFO("[Scene Manager] Load scene %s", name.c_str());

		m_scene->clear();
		const auto sceneFileName = core::utils::toStr("%s/scene/%s.scene", io::resource_manager::DATA_NAME.c_str(), name.c_str());
		std::ifstream file = std::ifstream(sceneFileName, std::ios::in);
		CAT_ASSERT(file.is_open());
		std::stringstream buffer;
		buffer << file.rdbuf();
		const auto data = buffer.str();
		auto sceneFromFile = rfl::json::read<std::shared_ptr<scene>>(data).value();
		//INFO(sceneFromFile->getName().c_str());
		m_scene = sceneFromFile;
		file.close();

		// TODO: .REMOVE
		const auto input = core::input_manager::getInstance();
		input->clearListeners();
		return true;
	}

	bool scene_manager::save()
	{
		INFO("[Scene Manager] Save scene");
		// TODO: Create folder if folder is not exist 

		const auto sceneFileName = core::utils::toStr("%s/scene/%s.scene", io::resource_manager::DATA_NAME.c_str(), m_scene->m_name.c_str());
		const std::string json_string = rfl::json::write(std::move(m_scene));
		
		std::ofstream file = std::ofstream(sceneFileName, std::ios::out);
		file << json_string;
		file.close();
		
		return true;
	}

	void scene_manager::render(graphics::renderer* renderer)
	{
		if (m_scene)
		{
			for (const auto& object : m_scene->getStorage())
			{
				auto gameObject = object.second.get();
				if (gameObject == nullptr)
					continue;

				gameObject->render(renderer);
			}
		}
	}

	void scene_manager::update(float DeltaTime)
	{
		if (m_scene)
		{
			for (const auto& object : m_scene->getStorage())
			{
				auto go = object.second.get();
				go->update(DeltaTime);
			}
		}
	}


	inline scene* scene_manager::getScene() const
	{
		return m_scene.get();
	}

	void scene_manager::clear()
	{
		m_scene->destroy();
		INFO("[Scene Manager] Current scene deleted");
	}

	scene_go_storage::iterator scene_manager::findGameObjectStr(const std::string& name)
	{
		scene_go_storage::iterator it = std::find_if(
			m_scene->m_storage.begin(),
			m_scene->m_storage.end(),
			[&name](std::pair<std::pair<std::string, uuids::uuid>,
				std::shared_ptr<game::game_object>> const& elem) {
					return elem.first.first == name;
			});

		return it;
	}

	scene_go_storage::iterator scene_manager::findGameObjectUUID(const uuids::uuid& uuid)
	{
		scene_go_storage::iterator it = std::find_if(
			m_scene->m_storage.begin(),
			m_scene->m_storage.end(),
			[&uuid](std::pair<std::pair<std::string, uuids::uuid>,
				std::shared_ptr<game::game_object>> const& elem) {
					return elem.first.second == uuid;
			});

		return it;
	}

	// FIX ME:  Reducing perfomance
	//			Need to make better algorithm, currently it is very bad way to do this.

	std::string scene_manager::makeGameObjectNameUnique(const std::string& name)
	{
		auto newName = std::string();

		// How much objects we found with the same name
		std::int32_t count = 0;
		while (true)
		{
			count != 0 ? newName = name + std::to_string(count) : newName = name;

			auto it = findGameObjectStr(newName);
			if (it == m_scene->m_storage.end())
			{
				break;
			}

			count++;
		}

		return newName;
	}

	void scene_manager::rename(game::game_object* go, const std::string& new_name)
	{
		CAT_NO_IMPL();
	}

	void scene_manager::del(game::game_object* go)
	{
		if (go == nullptr || go->getPrefix() == -1 ||
			go->getType() == CAT_ENGINE_GAMEOBJECT_TYPE)
		{
			ERR("[Scene Manager] Can't delete object");
			return;
		}

		const auto object_uuid = go->getUUID();
		const auto it = findGameObjectUUID(object_uuid.getID());
		if (it != m_scene->m_storage.end())
		{
			INFO("[Scene Manager] [%s] Delete object Name: %s UUID: %s",
				m_scene->m_name.c_str(),
				go->getName().c_str(),
				object_uuid.getIDStr().c_str());

			m_selected = go == m_selected ? nullptr : m_selected;
			m_scene->m_storage.erase(it);
			return;
		}
		
		ERR("[Scene Manager] Object is not found in scene storage");
	}

	void scene_manager::replace(game::game_object* first, game::game_object* second)
	{
		CAT_NO_IMPL();
	}
}