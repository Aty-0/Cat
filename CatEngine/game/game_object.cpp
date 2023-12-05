#include "game_object.h"
#include "graphics/renderer.h"

namespace cat::game
{
	game_object::game_object() :
		m_name(CAT_DEFAULT_GAMEOBJECT_NAME),
		m_type(CAT_DEFAULT_GAMEOBJECT_TYPE),
		m_prefix(CAT_DEFAULT_GAMEOBJECT_PREFIX),
		m_visible(true),
		m_enabled(true)
	{
		// This is the basic setup for game object 
		m_transform = create_component<components::transform>();
	}

	game_object::game_object(std::string name, std::string type, std::int32_t prefix, std::string uuid) :
		m_name(std::string()),
		m_type(std::string()),
		m_prefix(0),
		m_visible(true),
		m_enabled(true)
	{
		// Basic setup: set name, type and prefix 
		set_name(name);
		set_type(type);
		set_prefix(prefix);

		if (uuid == CAT_UUID_REGENERATE)
		{
			m_uuid.set(m_uuid.make_new());
		}
		else if (!uuid.empty())
		{
			// If uuid is invalid we are get assert
			m_uuid.set(uuid); 
		}

		onCreate();

		m_transform = create_component<components::transform>();
	}

	game_object::~game_object()
	{
		onDestroy();
		for (const auto component : m_components)
			component.second->onDestroy();

		m_transform = nullptr;
		m_components.clear();
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
					component.second->on_update(deltaTime);
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
					component.second->on_render(renderer);
				}
			}
		}
	}

	void game_object::set_name(std::string name)
	{
		name.empty() ? m_name = CAT_DEFAULT_GAMEOBJECT_NAME : m_name = name;
	}

	void game_object::set_type(std::string type)
	{
		type.empty() ? m_type = CAT_DEFAULT_GAMEOBJECT_TYPE : m_type = type;
	}

	void game_object::set_visible(bool visible)
	{
		m_visible = visible;

		const auto child = get_transform()->get_child();
		if (child != nullptr)
		{
			child->set_visible(visible);
		}
	}

	void game_object::set_enabled(bool enabled)
	{
		m_enabled = enabled;

		const auto child = get_transform()->get_child();
		if (child != nullptr)
		{
			child->set_enabled(enabled);
		}
	}

	void game_object::set_prefix(std::int32_t prefix)
	{
		m_prefix = prefix;
	}

	components::transform* game_object::get_transform() const
	{
		return m_transform;
	}

	components_list game_object::get_components() const
	{
		return m_components;
	}

	std::string game_object::get_name()   const
	{
		return m_name;
	}

	std::int32_t game_object::get_prefix() const
	{
		return m_prefix;
	}

	std::string game_object::get_type()   const
	{
		return m_type;
	}

	bool game_object::is_visible() const
	{
		return m_visible;
	}

	bool game_object::is_enabled() const
	{
		return m_enabled;
	}

	core::uuid_object game_object::get_uuid() const
	{
		return m_uuid;
	}
}