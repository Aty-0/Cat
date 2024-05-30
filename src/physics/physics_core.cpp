#include "physics_core.h"
#include "core/utils/logger.h"

#include "game/game_object.h"
#include "game/components/physical_body.h"

#include "graphics/renderer.h"

#include "physics/utils.h"
#include "physics/bp_layer_interfacy.h"
#include "physics/object_vs_bp_layer_filter.h"
#include "physics/object_layer_pair_filter.h"
#include "physics/joltDebugRenderer.h"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>

#include "core/utils/game_time.h"

namespace cat::physics
{
	physics_core::physics_core() 
	{

	}

	physics_core::~physics_core()
	{	

	}

	void physics_core::destroy()
	{
		VERB("physics_core destroy...");
		JPH::UnregisterTypes();

		cat::core::utils::safeDelete(m_temp_alloc);
		cat::core::utils::safeDelete(m_job_system);
		// Destroy the factory
		cat::core::utils::safeDelete(JPH::Factory::sInstance);
	}

	void physics_core::update(float deltaTime)
	{
		if (deltaTime == 0.0f || deltaTime > 1)
			return;

		// Step the world	
		static auto acc_time = 0.0f;
		const float phDeltaTime = 1.0f / 60.0f; 
		acc_time += deltaTime;
		std::int32_t steps = 1;
		while (acc_time > phDeltaTime)
		{
			acc_time -= phDeltaTime;
			m_physics_system->Update(steps * acc_time, steps, m_temp_alloc, m_job_system);
			steps++;
		}

	}

	void physics_core::init()
	{
		VERB("physics_core::init");

		JPH::Trace = OnTraceJPHCallback;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = OnAssertJPHCallback;)

		JPH::RegisterDefaultAllocator();
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

		m_temp_alloc = new JPH::TempAllocatorImpl(64 * 1024 * 1024);

		const auto threadCount = std::thread::hardware_concurrency() - 1;
		m_job_system = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, threadCount);

		// Add our filters
		static bp_layer_interfacу bp_layer_interface;
		static object_vs_bp_layer_filter object_vs_broadphase_layer_filter;
		static object_layer_pair_filter object_vs_object_layer_filter;
		// initialize physics system
		m_physics_system = new JPH::PhysicsSystem();
		
		m_renderer = new physics::joltDebugRenderer(graphics::renderer::getInstance());

		m_physics_system->Init(m_maxBodies, m_numBodyMutexes, m_maxBodyPairs, m_maxContactConstraints, 
			bp_layer_interface, object_vs_broadphase_layer_filter,
			object_vs_object_layer_filter);

		// set body interface from physics system 
		m_body_interface = &m_physics_system->GetBodyInterface();		
		m_physics_system->OptimizeBroadPhase();
		m_physics_system->SetGravity(JPH::Vec3(0, -9.81f, 0));
	}

	JPH::BodyInterface* physics_core::getBodyInterface() 
	{
		return m_body_interface;
	}

	JPH::PhysicsSystem* physics_core::getPhysicsSystem() 
	{
		return m_physics_system;
	}
}