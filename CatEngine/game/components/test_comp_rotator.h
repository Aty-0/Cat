#pragma once
#include "game/components/component.h"

namespace cat::game::components
{
	class CATENGINE_API test_comp_rotator : public component
	{
	public:
		test_comp_rotator();
		~test_comp_rotator();

		void test();
		void onUpdate(float delta_time) override;
	};
}