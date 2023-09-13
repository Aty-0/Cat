#pragma once
#include "scene_manager.h"

namespace cat::game::scene
{
	template<typename T>
	inline T* scene_manager::get_game_object_name(std::string name)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		const auto cur_scene = get_scene();

		// Without scene we can't continue 
		CAT_ASSERT(cur_scene != nullptr);
		try
		{
			auto storage = cur_scene->get_storage();
			for (auto it = storage.begin(); it != storage.end(); it++)
			{
				if (it->first.first == name)
				{
					return dynamic_cast<T*>(it->second.get());
				}

			}

			// We are check all objects...
			throw std::out_of_range("No object found");
		}
		catch (std::out_of_range)
		{
			VERB("scene_manager::get_game_object_name Error: Trying to get object %s but it is not exist", name.c_str());
		}
	
		return nullptr;
	}

	template<typename T>
	inline T* scene_manager::get_game_object_uuid(boost::uuids::uuid uuid)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		const auto cur_scene = get_scene();

		// Without scene we can't continue 
		CAT_ASSERT(cur_scene != nullptr);

		try
		{
			auto storage = cur_scene->get_storage();
			for (auto it = storage.begin(); it != storage.end(); it++)
			{
				if (it->first.second == uuid)
				{
					return dynamic_cast<T*>(it->second.get());
				}

			}

			// We are check all objects...
			throw std::out_of_range("No object found");
		}
		catch (std::out_of_range)
		{
			VERB("scene_manager::get_game_object_name Error: Trying to get object with uuid %s but it is not exist", core::uuid::convert_uuid_to_str(uuid).c_str());
		}

		return nullptr;
	}

	template<typename T>
	inline T* scene_manager::create_game_object(std::string name, std::string type, std::int32_t prefix, 
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		const auto cur_scene = get_scene();

		// Without scene we can't continue 
		CAT_ASSERT(cur_scene != nullptr);

		const auto object = new T(name, type, prefix, std::string());
		const auto transform = object->get_transform();

		// If we get there assertion fail it means something very bad happened. 
		CAT_ASSERT(transform);

		transform->set_position(position);
		transform->set_rotation(rotation);
		transform->set_scale(scale);

		return cur_scene->add<T>(object);
	}

	template<typename T>
	inline T* scene_manager::create_game_object_ninit()
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		const auto cur_scene = get_scene();

		// Without scene we can't continue 
		CAT_ASSERT(cur_scene != nullptr);

		const auto object = new T();
		return cur_scene->add<T>(object);
	}
}