#include "physics/bp_layer_interfacy.h"
#include "physics/layers.h"
#include "core/utils/logger.h"

namespace cat::physics
{
	bp_layer_interfacу::bp_layer_interfacу()
	{
		// Create a mapping table from object to broad phase layer
		m_objectToBroadPhase[layers::NON_MOVING] = BroadPhaseLayers::BP_NON_MOVING;
		m_objectToBroadPhase[layers::MOVING] = BroadPhaseLayers::BP_MOVING;
	}

	JPH::uint bp_layer_interfacу::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::BP_NUM_LAYERS;
	}

	JPH::BroadPhaseLayer bp_layer_interfacу::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
	{
		CAT_ASSERT(inLayer < layers::NUM_LAYERS);
		return m_objectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* bp_layer_interfacу::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::BP_NON_MOVING:	return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::BP_MOVING:		return "MOVING";
		default:
			FATAL("Physics: Unknown layer");
			return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
}