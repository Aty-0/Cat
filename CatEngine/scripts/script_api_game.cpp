#pragma once
#include "scripts/script_api_game.h"

// Game-Components modules
#include "game/game_object.h"
#include "game/components/component.h"
#include "game/components/drawable.h"
#include "game/components/test_comp_rotator.h"
#include "game/components/camera.h"
#include "game/components/physical_body.h"

// TODO: Maybe components need to relocate

namespace cat::scripts
{
	void add_game_api(sol::table& api)
	{
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

			// TODO: Can we use table for this ?
			"create_component_basic", &game::game_object::create_component<game::components::component>,
			"create_component_camera", &game::game_object::create_component<game::components::camera>,
			"create_component_test_comp_rotator", &game::game_object::create_component<game::components::test_comp_rotator>,
			"create_component_drawable", &game::game_object::create_component<game::components::drawable>,
			"create_component_physical_body", &game::game_object::create_component<game::components::physical_body>,

			"get_component_basic", &game::game_object::get_component<game::components::component>,
			"get_component_camera", &game::game_object::get_component<game::components::camera>,
			"get_component_test_comp_rotator", &game::game_object::get_component<game::components::test_comp_rotator>,
			"get_component_drawable", &game::game_object::get_component<game::components::drawable>,
			"get_component_physical_body", &game::game_object::get_component<game::components::physical_body>


		);

		api.new_usertype<game::components::component>("component",
			"get_name", &game::components::component::get_name,
			"get_owner", &game::components::component::get_owner,
			"get_uuid", &game::components::component::get_uuid,
			// Callbacks
			"onCreate", &game::components::component::onCreate,
			"onDestroy", &game::components::component::onDestroy
		);

		// TODO:
		api.new_usertype<game::components::camera>("camera");
		api.new_usertype<game::components::test_comp_rotator>("test_comp_rotator");

		api.new_enum("motion_type",
			"Static", game::components::physical_body::motion_type::Static,
			"Dynamic", game::components::physical_body::motion_type::Dynamic,
			"Kinematic", game::components::physical_body::motion_type::Kinematic);

		api.new_usertype<game::components::physical_body>("physic_body",
			"setMotionType", &game::components::physical_body::setMotionType);

		// TODO:
		api.new_usertype<game::components::drawable>("drawable",
			"set_color", &game::components::drawable::set_color,
			"set_texture", &game::components::drawable::set_texture
		);

		api.new_usertype<game::components::transform>("transform",
			"set_position", &game::components::transform::set_position,
			"set_rotation", &game::components::transform::set_rotation,
			"set_scale", &game::components::transform::set_scale,
			"set_velocity", &game::components::transform::set_velocity,
			"is_child_of", &game::components::transform::is_child_of,
			"set_parent", &game::components::transform::set_parent,
			"set_scale_factor", &game::components::transform::set_scale_factor,
			"get_child", &game::components::transform::get_child,
			"get_matrix_transformation", &game::components::transform::get_matrix_transformation,
			"get_name", &game::components::transform::get_name,
			"get_owner", &game::components::transform::get_owner,
			"get_parent", &game::components::transform::get_parent,
			"get_position", &game::components::transform::get_position,
			"get_rotation", &game::components::transform::get_rotation,
			"get_velocity", &game::components::transform::get_velocity,
			"get_scale", &game::components::transform::get_scale,
			"get_scale_factor", &game::components::transform::get_scale_factor
		);
	}
}