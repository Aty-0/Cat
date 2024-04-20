#pragma once
#include "scene_manager.h"

namespace cat::game::scene
{
	template<typename T>
	inline T* scene_manager::getGameObjectName(std::string name)
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
	inline T* scene_manager::getGameObjectUUID(uuids::uuid uuid)
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
				core::uuid_object::toStr(uuid).c_str());
		}

		return nullptr;
	}

	template<typename T>
	inline decltype(auto) scene_manager::createGameObject(std::string name, std::string type, std::int32_t prefix,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");
		
		// Without scene we can't continue 
		CAT_ASSERT(m_scene);

		const auto object = new T(name, type, prefix, std::string());
		const auto transform = object->getTransform();

		// If we get there assertion fail it means something very bad happened. 
		CAT_ASSERT(transform);

		transform->setPosition(position);
		transform->setRotation(rotation);
		transform->setScale(scale);

		return add<T>(object);
	}

	template<typename T>
	inline decltype(auto) scene_manager::createGameObjectWithoutInit()
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
		CAT_ASSERT(!(g->getName().empty() || g->getUUID().empty()));
		// Check on unique name 
		g->setName(makeGameObjectNameUnique(g->getName()));

		VERB("[Scene Manager] [%s] Add [%s] Name: %s UUID: %s %s",
			m_scene->m_name.c_str(), core::utils::getClassNameStr(g).c_str(),
			g->getName().c_str(), g->getUUID().getIDStr().c_str(),
			g->getTransform()->toStr().c_str());

		// Add object to storage
		const auto go_shared = std::make_shared<T>(*g);
		m_scene->m_storage.push_back(std::make_pair(std::make_pair(g->getName(), g->getUUID().getID()), go_shared));

		return go_shared.get();
	}

	template<typename T>
	inline decltype(auto) scene_manager::clone(T* go, std::string new_name)
	{
		static_assert(!std::is_base_of<T, game::game_object>::value || std::is_same<T, game::game_object>::value,
			"This is not GameObject or GameObject based class");

		VERB("[Scene Manager] [%s] Clone object Name: %s New name: %s", m_scene->m_name.c_str(),
			go->getUUID().getIDStr().c_str(),
			new_name.empty() ? "No" : new_name.c_str());

		const auto new_obj = new T(*go);
		const auto name = new_name.empty() ? new_obj->getName() + "_clone" : new_name;
		new_obj->init(name, new_obj->getType(), new_obj->getPrefix(), "REGENERATE");

		return add<T>(new_obj);
	}

}