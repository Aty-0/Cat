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
		core::utils::safe_delete(m_scene);
	}

	void scene_manager::create()
	{
		INFO("[SceneManager] Create new scene.");

		m_scene = new scene();

		create_game_object<game::game_object>(game::components::camera::EngineCameraName, CAT_ENGINE_GAMEOBJECT_TYPE, -1)->create_component<game::components::camera>();
		// TODO: Callbacks
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
		INFO("[SceneManager] Current scene deleted");
	}
}