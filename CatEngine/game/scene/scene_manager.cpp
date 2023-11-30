#include "scene_manager.h"
#include "game/components/camera.h"
#include "graphics/renderer.h"

namespace cat::game::scene
{
	scene_manager::scene_manager() :
		m_scene(nullptr)
	{

	}

	scene_manager::~scene_manager()
	{
		m_scene->destroy();
		core::utils::safe_delete(m_scene);
	}

	void scene_manager::create()
	{
		INFO("[SceneManager] Create new scene.");

		m_scene = new scene();

		create_game_object<game::game_object>(game::components::camera::EngineCameraName, CAT_ENGINE_GAMEOBJECT_TYPE, -1)->create_component<game::components::camera>();
		// TODO: Callbacks

		graphics::renderer::onImGuiRender.add(std::bind(&scene_manager::debug_render_imgui_window, this));
	}

	void scene_manager::debug_render_imgui_window()
	{
		ImGui::Begin("scene");

		for (const auto& object : m_scene->get_storage())
		{
			ImGui::Text("Name %s", object.second->get_name().c_str());
			ImGui::Text("uuid %s", object.second->get_uuid().get_id_str().c_str());
		}


		ImGui::End();
	}

	bool scene_manager::load(std::string name)
	{
		CAT_NO_IMPL();
		return true;
	}

	bool scene_manager::save()
	{
		CAT_NO_IMPL();
		return true;
	}

	void scene_manager::render(graphics::renderer* renderer)
	{
		if (m_scene)
		{
			for (const auto& object : m_scene->get_storage())
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
			for (const auto& object : m_scene->get_storage())
			{
				object.second.get()->update(DeltaTime);
			}
		}
	}


	inline scene* scene_manager::get_scene() const
	{
		return m_scene;
	}

	void scene_manager::clear()
	{
		m_scene->destroy();
		INFO("[Scene Manager] Current scene deleted");
	}

	scene_go_storage::iterator scene_manager::find_game_object_str(const std::string& name)
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

	scene_go_storage::iterator scene_manager::find_game_object_uuid(const uuids::uuid& uuid)
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

	std::string scene_manager::make_name_unique(const std::string& name)
	{
		auto newName = std::string();

		// How much objects we found with the same name
		auto count = 0;

		scene_go_storage::iterator it = { };

		while (true)
		{
			count != 0 ? newName = name + std::to_string(count) : newName = name;
			it = find_game_object_str(newName);

			if (it == m_scene->m_storage.end())
			{
				break;
			}

			count++;
		}

		return newName;
	}

	void scene_manager::rename(game::game_object* go, std::string new_name)
	{
		CAT_NO_IMPL();
	}

	void scene_manager::del(game::game_object* go)
	{
		if (go == nullptr)
		{
			ERR("[Scene Manager] Can't delete object because go is nullptr");
			return;
		}

		auto object_uuid = go->get_uuid();
		if (go->get_prefix() == -1 ||
			go->get_type() == CAT_ENGINE_GAMEOBJECT_TYPE)						
		{
			ERR("[Scene Manager] Can't delete object Name: UUID: %s",
				m_scene->m_name.c_str(),
				go->get_name(),
				object_uuid.get_id_str());
		}

		auto it = find_game_object_uuid(object_uuid.get_id());
		if (it != m_scene->m_storage.end())
		{
			INFO("[Scene Manager] Delete object Name: UUID: %s",
				m_scene->m_name.c_str(),
				go->get_name(),
				object_uuid.get_id_str());
		
			m_scene->m_storage.erase(it);
		}
		else
		{
			ERR("[Scene Manager] Object is not found in scene storage");
		}
	}

	void scene_manager::replace(game::game_object* go_first, game::game_object* go_second)
	{
		CAT_NO_IMPL();
	}
}