#include "scripts/script_api_3rd_party.h"

namespace cat::scripts
{
#ifndef CAT_DISABLE_LUA_CAT_API
	void add3rdPartyAPI(sol::table& api)
	{
		// TODO
		api.new_usertype<glm::mat2>("mat2");
		api.new_usertype<glm::mat3>("mat3");
		api.new_usertype<glm::mat4>("mat4");

		api.new_usertype<glm::vec1>("vec1",
			sol::constructors <glm::vec1(), glm::vec1(float)>(),
			"x", &glm::vec1::x);

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
	}
#endif //  CAT_DISABLE_LUA
}