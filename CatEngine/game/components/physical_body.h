#pragma once
#include "core/common.h"
#include "game/components/component.h"

#include <jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/MotionQuality.h>

namespace cat::physics
{
	class physics_core;
}

namespace cat::game::components
{
	class CATENGINE_API physical_body : public component
	{
	public:
		using motion_type = JPH::EMotionType;
		using motion_quality = JPH::EMotionQuality;

		physical_body();
		~physical_body();

		[[nodiscard]] inline JPH::BodyID getBodyId() const;

		void setMass(float mass);
		void setShape(JPH::Ref<JPH::Shape> quality);
		void setMotionQuality(motion_quality quality);
		void setMotionType(motion_type type);
		void setGravityFactor(float factor);
		void setFriction(float friction);

	private:
		JPH::BodyID m_id;
		JPH::Body* m_body;
		physics::physics_core* m_core;

	protected:
		// Create a basic body 
		void createBody();

		// Synchronize the game object transform position and the physics engine body position
		void updateBodyInterfacePosition();

		// Synchronize the game object transform velocity and the physics engine body velocity
		void updateBodyInterfaceVelocity();
	};
}