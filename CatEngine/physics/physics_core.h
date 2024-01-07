#pragma once
#include "core/common.h"

#include <jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

namespace cat::game
{
	class game_object;
}
namespace cat::physics
{
	class CATENGINE_API physics_core : public core::utils::singleton<physics_core>
	{
	public:
		physics_core();
		~physics_core();

		void init();
		void update(float deltaTime, game::game_object* go);
		
		[[nodiscard]] inline JPH::BodyInterface* getBodyInterface();
		[[nodiscard]] inline JPH::PhysicsSystem* getPhysicsSystem();

	private:
		// todo: increases when it will be needed
		const std::uint32_t m_maxBodies = 1024;
		const std::uint32_t m_maxContactConstraints = 1024;
		const std::uint32_t m_maxBodyPairs = 1024;
		const std::uint32_t m_numBodyMutexes = 0;
		
		JPH::PhysicsSystem* m_physics_system;
		JPH::BodyInterface* m_body_interface;
	};
}