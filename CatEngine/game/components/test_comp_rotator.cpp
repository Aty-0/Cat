#include "test_comp_rotator.h"
#include "game/game_object.h"

namespace cat::game::components
{
	test_comp_rotator::test_comp_rotator()
	{
		//onCreate.add(std::bind(&test_comp_rotator::test, this));
		onCreate += std::bind(&test_comp_rotator::test, this);
	}
	test_comp_rotator::~test_comp_rotator()
	{

	}

	void test_comp_rotator::test()
	{
		onCreate.remove(std::bind(&test_comp_rotator::test, this));
	}

	void test_comp_rotator::onUpdate(float delta_time)
	{
		const auto owner = getOwner();
		if (owner)
		{
			if (delta_time > 1)
				return;

			auto transform = owner->getTransform();
			static float angle = 1;
			angle += 15.0f * delta_time;
			//VERB("%f",angle);
			transform->setRotation(glm::vec3(angle, 0, 0));
			//transform->set_scale(glm::vec3(angle, 1, 1));
			static bool mode = false;
			static float x = 0.0f;

			if(mode == false)
				x += 1.0f * delta_time;
			else 
				x -= 1.0f * delta_time;

			if (x >= 3)
				mode = true;
			else if (x <= -3)
				mode = false;

			//transform->set_position(glm::vec3(x, 0, 0));
			transform->setScale(glm::vec3(1 - (x * 0.5), 1, 1));

		}
	}
}