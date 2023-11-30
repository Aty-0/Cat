#include "scripts/script_core.h"
#include "io/resource_manager.h"

#include "core/game_window.h"
#include "core/utils/game_time.h"
#include "core/engine.h"
#include "core/input_manager.h"
#include "core/callback_storage.h"


#include "game/scene/scene_manager.h"
#include "game/scene/scene.h"


#include "game/game_object.h"


#include "game/components/component.h"
#include "game/components/drawable.h"

namespace cat::scripts
{
	inline void info(const std::string& msg)
	{
		INFO("[LUA] %s", msg.c_str());
	}

	inline void verb(const std::string& msg)
	{
		VERB("[LUA] %s", msg.c_str());
	}

	inline void warn(const std::string& msg)
	{
		WARN("[LUA] %s", msg.c_str());
	}

	inline void err(const std::string& msg)
	{
		ERR("[LUA] %s", msg.c_str());
	}

	inline void fatal(const std::string& msg)
	{
		FATAL("[LUA] %s", msg.c_str());
	}

	inline void uuid_set_str(core::uuid_object uuid, std::string uuid_str)
	{
		uuid.set(uuid_str);
	}

	inline void uuid_set_uuid(core::uuid_object uuid, uuids::uuid id)
	{
		uuid.set(id);
	}

	typedef std::vector<std::pair<const char*, core::input_key_code>> full_keys_list;
	struct cat_keys_list
	{
		full_keys_list kl = core::utils::FULL_LIST_OF_KEYS;
	};

	// TODO: Replace 
	struct lua_keys_iterator_state 
	{
		full_keys_list::iterator it;
		full_keys_list::iterator last;

		lua_keys_iterator_state(cat_keys_list& key_list)
			: it(key_list.kl.begin()), last(key_list.kl.end())
		{

		}
	};

	std::tuple<sol::object, sol::object> keys_next(
		sol::user<lua_keys_iterator_state&> user_it_state,
		sol::this_state l) 
	{
		lua_keys_iterator_state& it_state = user_it_state;
		auto& it = it_state.it;
		if (it == it_state.last) 
		{
			return std::make_tuple(sol::object(sol::lua_nil),
				sol::object(sol::lua_nil));
		}
		auto itderef = *it;
		auto r = std::make_tuple(
			sol::object(l, sol::in_place, it->first),
			sol::object(l, sol::in_place, it->second));

		std::advance(it, 1);
		return r;
	}

	auto keys_pairs(cat_keys_list& key_list)
	{
		lua_keys_iterator_state it_state(key_list);

		return std::make_tuple(&keys_next,
			sol::user<lua_keys_iterator_state>(std::move(it_state)),
			sol::lua_nil);
	}


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
	
	void script_core::add_engine_api(sol::state& lua)
	{
		// Create namespace for our API
		sol::table api = lua.create_named_table("cat");

		// Add logger functions 
		api.set_function("info", info);
		api.set_function("warn", warn);
		api.set_function("err", err);
		api.set_function("fatal", fatal);
		api.set_function("verb", verb);
		
		// Add a core API		
		api.new_usertype<core::game_window>("game_window",
			"new", sol::no_constructor,
			"get_instance", &core::game_window::get_instance,
			"set_height", &core::game_window::set_height,
			"set_left", &core::game_window::set_left,
			"set_top", &core::game_window::set_top,
			"set_width", &core::game_window::set_width,
			"get_height", &core::game_window::get_height,
			"get_left", &core::game_window::get_left,
			"get_top", &core::game_window::get_top,
			"get_width", &core::game_window::get_width			
			);
		
		api.new_usertype<core::engine>("engine",
			"new", sol::no_constructor,
			"get_instance", &core::engine::get_instance,
			"destroy", &core::engine::destroy
		);

		api.new_usertype<core::utils::game_time>("game_time",
			"new", sol::no_constructor,
			"get_instance", &core::utils::game_time::get_instance,
			"get_delta_time", &core::utils::game_time::get_delta_time,
			"get_fps", &core::utils::game_time::get_fps
		);

		api.new_usertype<cat_keys_list>(
			"key_code", sol::meta_function::pairs, keys_pairs);
		
		api.new_enum("key_state",
			"Hold", core::input_key_state::Hold,
			"Press", core::input_key_state::Press,
			"Release", core::input_key_state::Release,
			"Unknown", core::input_key_state::Unknown,
			"Repeat",	core::input_key_state::Repeat 	// Don't use
			);
		api.new_enum("input_device",
			 "Joystick", core::input_device::Joystick,
			 "Keyboard", core::input_device::Keyboard,
			 "Mouse",	 core::input_device::Mouse,	 
			 "Unknown",	 core::input_device::Unknown 
		);

		api.new_usertype<core::input_manager>("input",
			"new", sol::no_constructor,
			"get_instance", &core::input_manager::get_instance,
			"add_listener", &core::input_manager::add_listener,
			"clear_listeners", &core::input_manager::clear_listeners,
			"get_key_state", &core::input_manager::get_key_state,
			"get_mouse_pos", &core::input_manager::get_mouse_pos,
			"unsubscribe_listener", &core::input_manager::unsubscribe_listener		
		);
		
		// Core -> Utils Api
		api.new_usertype<core::uuid_object>("uuid_object",
			"empty", &core::uuid_object::empty,
			"get_id", &core::uuid_object::get_id,
			"get_id_str", &core::uuid_object::get_id_str,
			"make_new", &core::uuid_object::make_new,
			"set", sol::overload(&uuid_set_str),
			"set", sol::overload(&uuid_set_uuid),
			"to_id", &core::uuid_object::to_id,
			"to_str", &core::uuid_object::to_str
		);

		api.new_usertype<core::callback_storage>("callback_storage",
			"add", &core::callback_storage::add,
			"clear", &core::callback_storage::clear,
			"run_all", &core::callback_storage::run_all,
			"remove", &core::callback_storage::remove,
			"update", &core::callback_storage::update		
		);

		// Game and other stuff related API
		api.new_usertype<game::game_object>("game_object", 
			sol::constructors<game::game_object(),
			game::game_object(std::string, std::string, std::int32_t, std::string)>(),
			"set_name", &game::game_object::set_name,
			"set_enabled", &game::game_object::set_enabled,
			"set_prefix", &game::game_object::set_prefix,
			"set_type", &game::game_object::set_type,
			"set_visible", &game::game_object::set_visible,
			"get_name", &game::game_object::get_name,
			"get_prefix", &game::game_object::get_prefix,
			"get_transform", &game::game_object::get_transform,
			"get_type", &game::game_object::get_type,
			"get_uuid", &game::game_object::get_uuid,
			"is_enabled", &game::game_object::is_enabled,
			"is_visible", &game::game_object::is_visible,
			// TODO: get_components
			
			// TODO: components
			"create_component_basic", &game::game_object::create_component<game::components::component>,
			"create_component_drawable", &game::game_object::create_component<game::components::drawable>
			
		);			
		
		api.new_usertype<game::components::component>("component",
			"get_name", &game::components::component::get_name,
			"get_owner", &game::components::component::get_owner,
			"get_uuid", &game::components::component::get_uuid,
			// Callbacks
			"onCreate", &game::components::component::onCreate,
			"onDestroy", &game::components::component::onDestroy			
		);
		
		api.new_usertype<game::components::drawable>("drawable",
			"set_texture", &game::components::drawable::set_texture);

		api.new_usertype<game::components::transform>("transform",
			"set_position", &game::components::transform::set_position,
			"set_rotation", &game::components::transform::set_rotation,
			"set_scale", &game::components::transform::set_scale,
			"set_scale_factor", &game::components::transform::set_scale_factor,
			"get_child", &game::components::transform::get_child,
			"get_matrix_transformation", &game::components::transform::get_matrix_transformation,
			"get_name", &game::components::transform::get_name,
			"get_owner", &game::components::transform::get_owner,
			"get_parent", &game::components::transform::get_parent,
			"get_position", &game::components::transform::get_position,
			"get_rotation", &game::components::transform::get_rotation,
			"get_scale", &game::components::transform::get_scale,
			"get_scale_factor", &game::components::transform::get_scale_factor
		);

		api.new_usertype<game::scene::scene_manager>("scene_manager",
			"new", sol::no_constructor,
			"get_instance", &game::scene::scene_manager::get_instance,
			"get_scene", &game::scene::scene_manager::get_scene,
			"clear", &game::scene::scene_manager::clear,
			"load", &game::scene::scene_manager::load,
			"save", &game::scene::scene_manager::save,
			"rename", &game::scene::scene_manager::rename,
			"del", &game::scene::scene_manager::del,
			"replace", &game::scene::scene_manager::replace,
			"make_name_unique", &game::scene::scene_manager::make_name_unique,
			// NOTE: If you added a new game object type, you should add it there 
			"create_game_object", &game::scene::scene_manager::create_game_object<game::game_object>,
			"create_game_object_ninit", &game::scene::scene_manager::create_game_object_ninit<game::game_object>,
			"get_game_object_name", &game::scene::scene_manager::get_game_object_name<game::game_object>,
			"get_game_object_uuid", &game::scene::scene_manager::get_game_object_uuid<game::game_object>
			);

		// 3rd party api
		
		// TODO:
		api.new_usertype<glm::mat2>("mat2");
		api.new_usertype<glm::mat3>("mat3");
		api.new_usertype<glm::mat4>("mat4");
		
		api.new_usertype<glm::vec1>("vec1", 
			sol::constructors <glm::vec1(), glm::vec1(float)>(),
			"x", & glm::vec1::x);

		api.new_usertype<glm::vec2>("vec2", 
			sol::constructors <glm::vec2(), glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y
		);

		api.new_usertype<glm::vec3>("vec3",
			sol::constructors <glm::vec3(), glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z
		);

		api.new_usertype<glm::vec4>("vec4",
			sol::constructors <glm::vec4(), glm::vec4(float, float, float, float)>(),
			"x", &glm::vec4::x,
			"y", &glm::vec4::y,
			"z", &glm::vec4::z,
			"w", &glm::vec4::w
		);
		// TODO: ...
	}

	// TODO: Error handler
	bool script_core::run_func(const char* name, const char* func_name)
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
	
	script* script_core::get_script(const char* name) const
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
			static const auto rm = io::resource_manager::get_instance();			
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
			
			add_engine_api(sc->lua);
			
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