#pragma once
#include "game/components/component.h"

namespace cat::game::components
{
	class CATENGINE_API test_comp_rotator : public component
	{
	public:

		void on_update(float delta_time) override;
	};
}