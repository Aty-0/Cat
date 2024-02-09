#pragma once
#include "core/common.h"
#include "game/components/component.h"

#include <jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

namespace cat::game::components
{
	class CATENGINE_API physical_body : public component
	{
	public:
		using motion_type = JPH::EMotionType;

		physical_body();
		~physical_body();

		[[nodiscard]] inline JPH::BodyID getBodyId() const;
		
		void setMotionType(motion_type type);

	private:
		JPH::BodyID m_id;
		JPH::Body* m_body;

	protected:
		// Create a basic body 
		void createBody();

		// Synchronize the game object transform position and the physics engine body position
		void updateBodyInterfacePosition();

		// Synchronize the game object transform velocity and the physics engine body velocity
		void updateBodyInterfaceVelocity();
	};
}