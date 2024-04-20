#pragma once
#include "game/components/drawable.h"

namespace cat::game::components
{
	class CATENGINE_API cube : public drawable
	{
	public:
		cube();

		void setTexture(const char* texture_name);

	};
}

