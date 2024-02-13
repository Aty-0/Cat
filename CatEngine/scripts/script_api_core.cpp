#include "scripts/script_api_core.h"

// Core modules
#include "core/game_window.h"
#include "core/uuid_object.h"
#include "core/utils/game_time.h"
#include "core/utils/logger.h"
#include "core/engine.h"
#include "core/input_manager.h"
#include "core/callback_storage.h"

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

	inline std::tuple<sol::object, sol::object> keys_next(
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

	inline auto keys_pairs(cat_keys_list& key_list)
	{
		lua_keys_iterator_state it_state(key_list);

		return std::make_tuple(&keys_next,
			sol::user<lua_keys_iterator_state>(std::move(it_state)),
			sol::lua_nil);
	}


	void add_core_api(sol::table& api)
	{
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
			"Unknown", core::input_key_state::Unknown
			// "Repeat", core::input_key_state::Repeat 	// Don't use
		);
		api.new_enum("input_device",
			"Joystick", core::input_device::Joystick,
			"Keyboard", core::input_device::Keyboard,
			"Mouse", core::input_device::Mouse,
			"Unknown", core::input_device::Unknown
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

	}
}