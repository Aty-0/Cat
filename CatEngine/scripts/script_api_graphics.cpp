#include "scripts/script_api_graphics.h"

// Graphics modules
#include "graphics/texture.h"
#include "graphics/shader.h"

namespace cat::scripts
{
	void add_graphics_api(sol::table& api)
	{
		api.new_usertype<graphics::texture>("texture");
		api.new_usertype<graphics::shader>("shader");
	}
}