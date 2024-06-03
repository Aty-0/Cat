#pragma once
#include "game/game_object.h"
#include "core/utils/logger.h"

namespace cat::game
{
	template<typename T>
	inline decltype(auto) game_object::addComponent(std::shared_ptr<T> component, std::uint32_t priority)
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		if (component == nullptr)
		{
			ERR("Component is empty");
			return nullptr;
		}

		component->setOwner(this);
		component->setName(core::utils::getClassNameStr(component));

		component->onCreate();

		VERB("game_object::add_component %s -> %s", getName().c_str(), component->getName().c_str());

		m_components.emplace(std::make_pair(component->m_uuid.getID(), component));
		return component.get();
	}

	// TODO: delete by priority ?

	template<typename T>
	inline void game_object::deleteComponent()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		const std::type_index type = typeid(T);
		components_list::iterator it = std::find_if(m_components.begin(), m_components.end(), [&type](std::pair<uuids::uuid, std::shared_ptr<components::component>> const& elem)
		{
			// If we try to get object based on component type
			if (dynamic_cast<T*>(elem.second.get()) != nullptr)
			{
				return true;
			}

			return typeid(elem.second) == type;
		});


		if (it == m_components.end())
		{
			ERR("game_object::delete_component %s Can't delete %s component is not found in game object!", getName().c_str(), typeid(T).name());
			return;
		}

	
		it->second->onDestroy();

		m_components.erase(it);
	}

	template<typename T>
	inline std::shared_ptr<T> game_object::initComponent(std::shared_ptr<T> component)
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		VERB("game_object::init_component in %s -> %s ", getName().c_str(), core::utils::getClassNameStr<T>().c_str());

		component->setOwner(this);
		component->setName(core::utils::getClassNameStr(component));
		component->m_uuid.set(component->m_uuid.make_new());

		// When name and owner is setted
		component->onCreate();

		return component;
	}

	template<typename T>
	inline decltype(auto) game_object::createComponent(std::uint32_t priority)
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		auto component = initComponent<T>(std::make_shared<T>());	
		m_components.push_back(std::make_pair(component->m_uuid.getID(), component));

		VERB("game_object::create_component %s -> %s", getName().c_str(), component->getName().c_str());
		return component.get();
	}

	template<typename T>
	inline T* game_object::getComponent()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		const std::type_index type = typeid(T);
		components_list::iterator it = std::find_if(m_components.begin(), m_components.end(), [&type](std::pair<uuids::uuid, std::shared_ptr<components::component>> const& elem)
		{
			// If we try to get object based on component type
			if (dynamic_cast<T*>(elem.second.get()) != nullptr)
			{
				return true;
			}

			return typeid(elem.second.get()) == type;
		});

		return dynamic_cast<T*>(it->second.get());
	}

	template<typename T>
	inline T* game_object::getComponentInChildren()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		if (m_transform->getChild() != nullptr)
		{
			return m_transform->getChild()->getComponent<T>();
		}

		return nullptr;
	}

	template<typename T>
	inline T* game_object::getComponentInParent()
	{
		static_assert(!std::is_base_of<T, components::component>::value || std::is_same<T, components::component>::value,
			"This is not Component or Component based class");

		if (m_transform->getParent() != nullptr)
		{
			return m_transform->getParent()->getComponent<T>();
		}

		return nullptr;
	}
}