#include "object_layer_pair_filter.h"
#include "physics/layers.h"
#include "core/utils/logger.h"

namespace cat::physics
{	
	bool object_layer_pair_filter::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const 
	{
		switch (inObject1)
		{
		case layers::NON_MOVING:
			return inObject2 == layers::MOVING; // Non moving only collides with moving
		case layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
}