#include "scripts/script_core.h"
#include "core/utils/logger.h"

#include "io/resource_manager.h"

#include "scripts/script_api_core.h"
#include "scripts/script_api_game.h"
#include "scripts/script_api_graphics.h"
#include "scripts/script_api_scene.h"
#include "scripts/script_api_3rd_party.h"

namespace cat::scripts
{
	script_core::script_core()
	{

	}

	script_core::~script_core()
	{
		clear();
	}
	
	void script_core::clear()
	{
		VERB("Script Core: Clear");
		m_scripts.clear();
		m_scripts.shrink_to_fit();
	}
	
	void script_core::addCatAPI(sol::state& lua)
	{
		// Create namespace for our API
		sol::table api = lua.create_named_table("cat");
#ifndef CAT_DISABLE_LUA_CAT_API
		addCoreAPI(api);
		addGameAPI(api);
		addGraphicsAPI(api);
		addSceneAPI(api);
		add3rdPartyAPI(api);
#endif
	}

	// TODO: Error handler
	bool script_core::runFunc(const char* name, const char* func_name)
	{
		VERB("Script Core: Run func %s", name);
		const auto it = std::find_if(m_scripts.begin(), m_scripts.end(), [name](std::pair<std::string, script*> const& el)
		{
			return el.first == name;
		});

		if (it == m_scripts.end())
		{
			ERR("Script Core: Can't find script with %s name", name);
			return false;
		}

		auto result = it->second->lua[func_name].call();
		if (!result.valid())
		{
			sol::error err = result;
			ERR("[LUA] %s", err.what());

			return false;
		}
		return true;
	}
	
	script* script_core::getScript(const char* name) const
	{
		const auto it = std::find_if(m_scripts.begin(), m_scripts.end(), [name](std::pair<std::string, script*> const& el)
			{
				return el.first == name;
			});
		
		if (it == m_scripts.end())
			return nullptr;

		return it->second;
	}

	bool script_core::run(const char* name, bool safe)
	{
		VERB("Script Core: Load script %s", name);
		const auto it = std::find_if(m_scripts.begin(), m_scripts.end(), [name](std::pair<std::string, script*> const& el)
		{
			return el.first == name;
		});

		if (it == m_scripts.end())
		{
			static const auto rm = io::resource_manager::getInstance();			
			auto data = rm->read<const char*, script>(name, { "lua" });
			
			// Script was not found or it is empty		
			if (data == NULL ||
				(data && !data[0]))
			{
				ERR("Script Core: File is not found or empty!");
				return false;
			}

			auto sc = new script();
			
			// TODO & FIXME: Add more if needed 
			sc->lua.open_libraries(sol::lib::base, 
				sol::lib::math, 
				sol::lib::string,
				sol::lib::io,
				sol::lib::table);
			
			addCatAPI(sc->lua);
			
			try
			{
				if(safe)
					const auto result = sc->lua.safe_script(data);
				else
					const auto result = sc->lua.script(data);

				// if (!result.valid())
				// {
				// 	sol::error err = result;
				// 	ERR("LUA: %s", err.what());
				// 
				// 	return false;
				// }		
			}
			catch (sol::error& ex)
			{
				ERR("LUA: %s", ex.what());

				return false;
			}

			m_scripts.push_back(std::move(std::make_pair(std::string(name), sc)));
		}
		else
		{
			ERR("Script core: This script is already loaded! %s", name);
			return false;
		}

		return true;
	}

}