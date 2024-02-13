#include "scripts/script_api_scene.h"

// Scene or related to scene modules
#include "game/game_object.h"
#include "game/scene/scene_manager.h"
#include "game/scene/scene.h"

namespace cat::scripts
{
	void add_scene_api(sol::table& api)
	{
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

	}
}