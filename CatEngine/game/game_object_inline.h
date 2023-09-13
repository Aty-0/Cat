#pragma once
#include "game/game_object.h"
#include "core/utils/logger.h"

namespace cat::game
{
	template<typename T>
	inline T* game_object::add_component(T* component, std::uint32_t priority)
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		if (component == nullptr)
		{
			ERR("Component is empty");
			return nullptr;
		}

		component->set_owner(this);
		component->set_name(core::utils::get_class_name_string(component));

		component->onCreate();

		VERB("game_object::add_component %s -> %s", get_name().c_str(), component->get_name().c_str());

		m_components.push_back(std::make_pair(std::make_pair(component->get_uuid(), std::type_index(typeid(T))), component));
		return component;
	}

	// TODO: delete by priority ?

	template<typename T>
	inline void game_object::delete_component()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		std::type_index type = typeid(T);

		components_list::iterator it = std::find_if(m_components.begin(), m_components.end(), [&type](std::pair<std::pair<boost::uuids::uuid,
			std::type_index>, components::component*> const& elem)
			{
				// If we try to get object based on component type
				if (dynamic_cast<T*>(elem.second) != nullptr)
				{
					return true;
				}

				return elem.first.second == type;
			});

		if (it == m_components.end())
		{
			ERR("game_object::delete_component %s Can't delete %s component is not found in game object!", get_name().c_str(), typeid(T).name());
			return;
		}

	
		it->second->onDestroy();

		m_components.erase(it);
	}

	template<typename T>
	inline T* game_object::init_component(T* component)
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		VERB("game_object::init_component in %s -> %s ", get_name().c_str(), core::utils::get_class_name_string<T>().c_str());

		component = new T();
		component->set_owner(this);
		component->set_name(core::utils::get_class_name_string(component));
		component->generate_uuid();

		// When name and owner is setted
		component->onCreate();

		return component;
	}

	template<typename T>
	inline T* game_object::create_component(std::uint32_t priority)
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		auto component = new T();

		component->set_owner(this);
		component->set_name(core::utils::get_class_name_string(component));
		component->generate_uuid();

		// When name and owner is setted
		component->onCreate();

		m_components.push_back(std::make_pair(std::make_pair(component->get_uuid(), std::type_index(typeid(*component))), component));

		INFO("game_object::create_component %s -> %s", get_name().c_str(), component->get_name().c_str());
		return component;
	}

	template<typename T>
	inline [[nodiscard]] T* game_object::get_component()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		std::type_index type = typeid(T);

		components_list::iterator it = std::find_if(m_components.begin(), m_components.end(), [&type](std::pair<std::pair<boost::uuids::uuid,
			std::type_index>, components::component*> const& elem)
			{
				// If we try to get object based on component type
				if (dynamic_cast<T*>(elem.second) != nullptr)
				{
					return true;
				}

				return elem.first.second == type;
			});

		if (it == m_components.end())
		{
			return nullptr;
		}

		return dynamic_cast<T*>(it->second);
	}

	template<typename T>
	inline [[nodiscard]] T* game_object::get_component_in_children()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		if (m_transform->get_child() != nullptr)
		{
			return m_transform->get_child()->get_component<T>();
		}

		return nullptr;
	}

	template<typename T>
	inline [[nodiscard]] T* game_object::get_component_in_parent()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		if (m_transform->get_parent() != nullptr)
		{
			return m_transform->get_parent()->get_component<T>();
		}

		return nullptr;
	}
}