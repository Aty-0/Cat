#include "component.h"
#include "core/engine.h"
#include "graphics/renderer.h"
#include "game/game_object.h"

namespace cat::game::components
{
	component::component() :
		m_owner(nullptr),
		m_select(false),
		m_name(std::string())
	{

	}

	component::~component()
	{
		m_owner = nullptr;
		m_select = false;
		m_name = std::string();
	}

	void component::setName(const std::string& name)
	{
		m_name = name;
	}

	void component::setOwner(game_object* go)
	{
		m_owner = go;
	}
	
	std::string component::getName() const
	{
		return m_name;
	}

	game_object* component::getOwner() const
	{
		return m_owner;
	}

	core::uuid_object component::getUUID() const
	{
		return m_uuid;
	}
}