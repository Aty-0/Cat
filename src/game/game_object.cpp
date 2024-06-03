#include "game_object.h"
#include "graphics/renderer.h"

namespace cat::game
{
	game_object::game_object() :
		m_name(CAT_DEFAULT_GAMEOBJECT_NAME),
		m_type(CAT_DEFAULT_GAMEOBJECT_TYPE),
		m_prefix(CAT_DEFAULT_GAMEOBJECT_PREFIX),
		m_visible(true),
		m_enabled(true),
		m_select(false)
	{
		// This is the basic setup for game object 
		m_transform = createComponent<components::transform>();
	}

	game_object::game_object(std::string name, std::string type, std::int32_t prefix, std::string uuid) :
		m_name(std::string()),
		m_type(std::string()),
		m_prefix(0),
		m_visible(true),
		m_enabled(true),
		m_select(false)
	{
		// Basic setup: set name, type and prefix 
		setName(name);
		setType(type);
		setPrefix(prefix);
		m_uuid.set(uuid == CAT_UUID_REGENERATE || uuid == "" ? m_uuid.make_new() : core::uuid_object::toID(uuid));

		onCreate();
		m_transform = createComponent<components::transform>();
	}

	game_object::~game_object()
	{
		onDestroy();
		for (const auto component : m_components)
		{
			component.second->onDestroy();
		}

		m_transform = nullptr;
		m_components.clear();
		m_components.shrink_to_fit();

		m_type.clear();
		m_name.clear();
		m_prefix = 0;
	}

	void game_object::update(float deltaTime)
	{
		if (m_enabled == true)
		{
			for (const auto component : m_components)
			{
				if (component.second != nullptr)
				{
					component.second->onUpdate(deltaTime);
				}
			}
		}
	}

	void game_object::render(graphics::renderer* renderer)
	{
		if (m_visible == true)
		{
			for (const auto component : m_components)
			{
				if (component.second != nullptr)
				{
					component.second->onRender(renderer);
				}
			}
		}
	}

	void game_object::setName(std::string name)
	{
		name.empty() ? m_name = CAT_DEFAULT_GAMEOBJECT_NAME : m_name = name;
	}

	void game_object::setType(std::string type)
	{
		type.empty() ? m_type = CAT_DEFAULT_GAMEOBJECT_TYPE : m_type = type;
	}

	void game_object::setVisible(bool visible)
	{
		m_visible = visible;

		const auto child = getTransform()->getChild();
		if (child != nullptr)
		{
			child->setVisible(visible);
		}
	}

	void game_object::setEnabled(bool enabled)
	{
		m_enabled = enabled;

		const auto child = getTransform()->getChild();
		if (child != nullptr)
		{
			child->setEnabled(enabled);
		}
	}

	void game_object::setPrefix(std::int32_t prefix)
	{
		m_prefix = prefix;
	}

	components::transform* game_object::getTransform() const
	{
		return m_transform;
	}

	game_object::components_list game_object::getComponents() const
	{
		return m_components;
	}

	std::string game_object::getName()   const
	{
		return m_name;
	}

	std::int32_t game_object::getPrefix() const
	{
		return m_prefix;
	}

	std::string game_object::getType()   const
	{
		return m_type;
	}

	bool game_object::isVisible() const
	{
		return m_visible;
	}

	bool game_object::isEnabled() const
	{
		return m_enabled;
	}

	core::uuid_object game_object::getUUID() const
	{
		return m_uuid;
	}
}