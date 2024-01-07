#pragma once
#include "core/common.h"
#include "game/components/component.h"

#include <jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

namespace cat::game::components
{
	// todo: collider is a friend for transform 

	class CATENGINE_API physic_body : public component
	{
	public:
		physic_body();
		~physic_body();

		[[nodiscard]] inline JPH::BodyID getBodyId() const;

	private:
		JPH::BodyID m_id;
		JPH::Body* m_body;

	protected:
		void createBody();
		void addPosCallback();
	};
}