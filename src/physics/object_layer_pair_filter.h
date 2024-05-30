#pragma once
#include "core/common.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace cat::physics
{
	class object_layer_pair_filter : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
		
	};
}
