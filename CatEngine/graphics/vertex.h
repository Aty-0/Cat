#pragma once
#include "core/common.h"

namespace cat::graphics
{
	struct CATENGINE_API vertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 uv;
	};
}