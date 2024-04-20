#pragma once
#include "core/common.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace cat::physics
{
	class bp_layer_interfacу final : public JPH::BroadPhaseLayerInterface
	{
	public:
		bp_layer_interfacу();
		
		virtual JPH::uint GetNumBroadPhaseLayers() const override;

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer m_objectToBroadPhase[2]; // FIX ME: NUM COUNT
	};
}
