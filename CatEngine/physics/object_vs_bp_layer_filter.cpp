#include "object_vs_bp_layer_filter.h"
#include "physics/layers.h"
#include "core/utils/logger.h"

namespace cat::physics
{
	bool object_vs_bp_layer_filter::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
	{
		switch (inLayer1)
		{
		case layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::BP_MOVING;
		case layers::MOVING:
			return true;
		default:
			FATAL("Physics: Unknown layer");
			return false;
		}
	}
}
