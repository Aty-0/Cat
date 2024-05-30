#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace cat::physics
{
	// TODO: Maybe using a table will be better 
	namespace layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;

		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	}
}

namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer BP_NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer BP_MOVING(1);
	static constexpr JPH::uint BP_NUM_LAYERS(2);
};