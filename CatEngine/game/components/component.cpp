#include "component.h"
#include "core/engine.h"
#include "graphics/renderer.h"
#include "game/game_object.h"

namespace cat::game::components
{
	component::component() :
		m_owner(nullptr),
		m_isSelected(false),
		m_name(std::string())
	{

	}

	component::~component()
	{
		m_owner = nullptr;
		m_isSelected = false;
		m_name = std::string();
	}

	void component::set_name(std::string name)
	{
		m_name = name;
	}

	void component::set_owner(game_object* go)
	{
		m_owner = go;
	}
	
	std::string component::get_name() const
	{
		return m_name;
	}

	game_object* component::get_owner() const
	{
		return m_owner;
	}

	core::uuid_object component::get_uuid() const
	{
		return m_uuid;
	}
}