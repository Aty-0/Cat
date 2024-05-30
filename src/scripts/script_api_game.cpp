#pragma once
#include "scripts/script_api_game.h"

// Game-Components modules
#include "game/game_object.h"
#include "game/components/component.h"
#include "game/components/drawable.h"
#include "game/components/sprite.h"
#include "game/components/test_comp_rotator.h"
#include "game/components/camera.h"
#include "game/components/physical_body.h"
#include "game/components/cube.h"

// TODO: Maybe components needs to relocate

namespace cat::scripts
{
#ifndef CAT_DISABLE_LUA_CAT_API
	void addGameAPI(sol::table& api)
	{
		// Game and other stuff related API
		api.new_usertype<game::game_object>("game_object",
			sol::constructors<game::game_object(),
			game::game_object(std::string, std::string, std::int32_t, std::string)>(),
			"set_name", &game::game_object::setName,
			"set_enabled", &game::game_object::setEnabled,
			"set_prefix", &game::game_object::setPrefix,
			"set_type", &game::game_object::setType,
			"set_visible", &game::game_object::setVisible,
			"get_name", &game::game_object::getName,
			"get_prefix", &game::game_object::getPrefix,
			"get_transform", &game::game_object::getTransform,
			"get_type", &game::game_object::getType,
			"get_uuid", &game::game_object::getUUID,
			"is_enabled", &game::game_object::isEnabled,
			"is_visible", &game::game_object::isVisible,

			// TODO: Can we use table for this ?
			"create_component_basic", &game::game_object::createComponent<game::components::component>,
			"create_component_camera", &game::game_object::createComponent<game::components::camera>,
			"create_component_drawable", &game::game_object::createComponent<game::components::drawable>,
			"create_component_physical_body", &game::game_object::createComponent<game::components::physical_body>,
			"create_component_sprite", &game::game_object::createComponent<game::components::sprite>,
			"create_component_cube", &game::game_object::createComponent<game::components::cube>,

			"get_component_basic", &game::game_object::getComponent<game::components::component>,
			"get_component_camera", &game::game_object::getComponent<game::components::camera>,
			"get_component_drawable", &game::game_object::getComponent<game::components::drawable>,
			"get_component_physical_body", &game::game_object::getComponent<game::components::physical_body>,
			"get_component_sprite", &game::game_object::getComponent<game::components::sprite>,
			"get_component_cube", &game::game_object::getComponent<game::components::cube>


		);

		api.new_usertype<game::components::component>("component",
			"get_name", &game::components::component::getName,
			"get_owner", &game::components::component::getOwner,
			"get_uuid", &game::components::component::getUUID,
			// Callbacks
			"onCreate", &game::components::component::onCreate,
			"onDestroy", &game::components::component::onDestroy
		);

		// TODO:
		api.new_usertype<game::components::camera>("camera", 
			"getFront", &game::components::camera::getFront,
			"getSpeed", &game::components::camera::getSpeed
		);

		api.new_usertype<game::components::test_comp_rotator>("test_comp_rotator");

		api.new_enum("motion_quality",
			"Discrete", game::components::physical_body::motion_quality::Discrete,
			"LinearCast", game::components::physical_body::motion_quality::LinearCast);

		api.new_enum("motion_type",
			"Static", game::components::physical_body::motion_type::Static,
			"Dynamic", game::components::physical_body::motion_type::Dynamic,
			"Kinematic", game::components::physical_body::motion_type::Kinematic);

		api.new_usertype<game::components::physical_body>("physical_body",
			"setMotionType", &game::components::physical_body::setMotionType,
			"setMass", &game::components::physical_body::setMass,
			"setFriction", &game::components::physical_body::setFriction,
			"setGravityFactor", &game::components::physical_body::setGravityFactor,
			"setMotionQuality", &game::components::physical_body::setMotionQuality
			// TODO: Shape
		);

		// TODO:
		api.new_usertype<game::components::drawable>("drawable",
			"set_color", &game::components::drawable::setColor
		);

		api.new_usertype<game::components::sprite>("sprite",
			"set_texture", &game::components::sprite::setTexture
		);

		api.new_usertype<game::components::cube>("cube",
			"set_texture", &game::components::cube::setTexture
		);

		api.new_usertype<game::components::transform>("transform",
			"set_position", &game::components::transform::setPosition,
			"set_rotation", &game::components::transform::setRotation,
			"set_scale", &game::components::transform::setScale,
			"set_velocity", &game::components::transform::setVelocity,
			"is_child_of", &game::components::transform::isChildOf,
			"set_parent", &game::components::transform::setParent,
			"set_scale_factor", &game::components::transform::setScaleFactor,
			"get_child", &game::components::transform::getChild,
			"get_matrix_transformation", &game::components::transform::getTransformation,
			"get_name", &game::components::transform::getName,
			"get_owner", &game::components::transform::getOwner,
			"get_parent", &game::components::transform::getParent,
			"get_position", &game::components::transform::getPosition,
			"get_rotation", &game::components::transform::getRotation,
			"get_velocity", &game::components::transform::getVelocity,
			"get_scale", &game::components::transform::getScale,
			"get_scale_factor", &game::components::transform::getScaleFactor
		);
	}
#endif
}