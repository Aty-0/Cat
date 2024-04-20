#include "scripts/script_api_graphics.h"

// Graphics modules
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"

namespace cat::scripts
{
#ifndef CAT_DISABLE_LUA_CAT_API
	void addGraphicsAPI(sol::table& api)
	{
		api.new_usertype<graphics::texture>("texture");
		api.new_usertype<graphics::shader>("shader");

		api.new_usertype<graphics::renderer>("renderer", 
			"new", sol::no_constructor,
			"get_instance", &graphics::renderer::getInstance, 
			"debugDrawLine", &graphics::renderer::debugDrawLine
			);
	}
#endif
}