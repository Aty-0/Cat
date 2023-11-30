#pragma once
#include "scene_manager.h"

namespace cat::game::scene
{
	template<typename T>
	inline T* scene_manager::get_game_object_name(std::string name)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		// Without scene we can't continue 
		CAT_ASSERT(m_scene);

		try
		{
			auto storage = m_scene->m_storage;
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
	inline T* scene_manager::get_game_object_uuid(uuids::uuid uuid)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");
		
		// Without scene we can't continue 
		CAT_ASSERT(m_scene);

		try
		{
			auto storage = m_scene->m_storage;
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
			VERB("scene_manager::get_game_object_name Error: Trying to get object with uuid %s but it is not exist", 
				core::uuid_object::to_str(uuid).c_str());
		}

		return nullptr;
	}

	template<typename T>
	inline T* scene_manager::create_game_object(std::string name, std::string type, std::int32_t prefix, 
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");
		
		// Without scene we can't continue 
		CAT_ASSERT(m_scene);

		const auto object = new T(name, type, prefix, std::string());
		const auto transform = object->get_transform();

		// If we get there assertion fail it means something very bad happened. 
		CAT_ASSERT(transform);

		transform->set_position(position);
		transform->set_rotation(rotation);
		transform->set_scale(scale);

		return add<T>(object);
	}

	template<typename T>
	inline T* scene_manager::create_game_object_ninit()
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");
		
		// Without scene we can't continue 
		CAT_ASSERT(m_scene);

		const auto object = new T();
		return add<T>(object);
	}

	template<typename T>
	inline decltype(auto) scene_manager::add(T* g)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value, "This is not GameObject or GameObject based class");
		CAT_ASSERT(m_scene);
		CAT_ASSERT(g);
		// Can't add game object if it doesn't have name or own id
		CAT_ASSERT(!(g->get_name().empty() || g->get_uuid().empty()));
		// Check on unique name 
		g->set_name(make_name_unique(g->get_name()));

		VERB("[Scene Manager] [%s] Add [%s] Name: %s UUID: %s at Position (%f %f %f)",
			m_scene->m_name.c_str(), core::utils::get_class_name_string(g).c_str(),
			g->get_name().c_str(), g->get_uuid().get_id_str().c_str(),
			g->get_transform()->get_position().x, g->get_transform()->get_position().y, g->get_transform()->get_position().z);

		// Add object to storage
		const auto go_shared = std::make_shared<T>(*g);
		m_scene->m_storage.push_back(std::move(std::make_pair(std::make_pair(g->get_name(), g->get_uuid().get_id()), go_shared)));

		const auto object = go_shared.get();
		return object;
	}

	template<typename T>
	inline decltype(auto) scene_manager::clone(T* go, std::string new_name)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		VERB("[Scene Manager] [%s] Clone object Name: %s New name: %s", m_scene->m_name.c_str(),
			go->get_uuid().get_id_str().c_str(),
			new_name.empty() ? "No" : new_name.c_str());

		const auto new_obj = new T(*go);
		const auto name = new_name.empty() ? new_obj->get_name() + "_clone" : new_name;
		new_obj->init(name, new_obj->get_type(), new_obj->get_prefix(), "REGENERATE");

		return add<T>(new_obj);
	}

}