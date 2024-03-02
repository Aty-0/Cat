#pragma once
#include "game/components/drawable.h"

namespace cat::game::components
{
	class CATENGINE_API sprite : public drawable
	{
	public:
		sprite();

		void set_texture(const char* texture_name);

	private:
		void rescale();
	};
}

