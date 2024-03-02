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

		void destroy();
		void init();
		void update(float deltaTime, game::game_object* go);
		
		[[nodiscard]] inline JPH::BodyInterface* getBodyInterface();
		[[nodiscard]] inline JPH::PhysicsSystem* getPhysicsSystem();

	private:
		const std::uint32_t m_maxBodies = 10240;
		const std::uint32_t m_maxContactConstraints = 20480;
		const std::uint32_t m_maxBodyPairs = 65536;
		const std::uint32_t m_numBodyMutexes = 0;
		
		JPH::PhysicsSystem* m_physics_system;
		JPH::BodyInterface* m_body_interface;
		JPH::TempAllocatorImpl* m_temp_alloc;
		JPH::JobSystemThreadPool* m_job_system;
	};
}