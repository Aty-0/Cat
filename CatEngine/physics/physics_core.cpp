#include "physics_core.h"
#include "core/utils/logger.h"

#include "game/game_object.h"
#include "game/components/physical_body.h"

#include "physics/utils.h"
#include "physics/bp_layer_interfacу.h"
#include "physics/object_vs_bp_layer_filter.h"
#include "physics/object_layer_pair_filter.h"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>



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

		cat::core::utils::safe_delete(m_temp_alloc);
		cat::core::utils::safe_delete(m_job_system);
		// Destroy the factory
		cat::core::utils::safe_delete(JPH::Factory::sInstance);
	}

	void physics_core::update(float deltaTime, game::game_object* go)
	{
		if (deltaTime == 0.0f)
			return;

		const auto phbody = go->get_component<game::components::physical_body>();
		if (phbody == nullptr)
			return;

		const auto id = phbody->getBodyId();
		
		//if (m_body_interface->IsActive(id))
		{
			JPH::RVec3 position = m_body_interface->GetCenterOfMassPosition(id);
			JPH::Vec3 velocity = m_body_interface->GetLinearVelocity(id);
			JPH::Vec3 rotation_in_euler = m_body_interface->GetRotation(id).GetEulerAngles();
			
			const auto transform = go->get_transform();
			transform->m_position = { position.GetX(), position.GetY(), position.GetZ() };
			transform->m_velocity = { velocity.GetX(), velocity.GetY(), velocity.GetZ() };
			transform->m_rotation = { glm::degrees(rotation_in_euler.GetX()), 
				glm::degrees(rotation_in_euler.GetY()), glm::degrees(rotation_in_euler.GetZ()) };
			
			// Step the world
			m_physics_system->Update(deltaTime, 1, m_temp_alloc, m_job_system);
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

		m_temp_alloc = new JPH::TempAllocatorImpl(32 * 1024 * 1024);

		const auto threadCount = std::thread::hardware_concurrency() - 1;
		m_job_system = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, threadCount);

		// Add our filters
		static bp_layer_interfacу bp_layer_interface;
		static object_vs_bp_layer_filter object_vs_broadphase_layer_filter;
		static object_layer_pair_filter object_vs_object_layer_filter;
		// initialize physics system
		m_physics_system = new JPH::PhysicsSystem();
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