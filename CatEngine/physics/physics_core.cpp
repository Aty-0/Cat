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
		VERB("physics_core::~physics_core");

		JPH::UnregisterTypes();
		// Destroy the factory
		cat::core::utils::safe_delete(JPH::Factory::sInstance);
	}

	void physics_core::update(float deltaTime, game::game_object* go)
	{
		const auto phbody = go->get_component<game::components::physical_body>();
		if (phbody == nullptr)
			return;

		const auto id = phbody->getBodyId();

		std::int32_t step = 0;
		m_physics_system->OptimizeBroadPhase();
		if (m_body_interface->IsActive(id))
		{			
			step++;
			
			// Output current position and velocity of the sphere
			JPH::RVec3 position = m_body_interface->GetCenterOfMassPosition(id);
			JPH::Vec3 velocity = m_body_interface->GetLinearVelocity(id);
			JPH::Vec3 rotation_in_euler = m_body_interface->GetRotation(id).GetEulerAngles();
			
			go->get_transform()->m_position = { position.GetX(), position.GetY(), position.GetZ() };
			go->get_transform()->m_velocity = { velocity.GetX(), velocity.GetY(), velocity.GetZ() };
			go->get_transform()->m_rotation = { rotation_in_euler.GetX(), rotation_in_euler.GetY(), rotation_in_euler.GetZ() };
			
			//VERB("step %i posx: %f posy: %f posz: %f velx: %f vely: %f velz: %f", step,
			//	position.GetX(), position.GetY(), position.GetZ(),
			//	velocity.GetX(), velocity.GetY(), velocity.GetZ());

			const auto collSteps = 1.0f;
			const auto phDeltaTime = deltaTime; 

			
			// FIXME: 
			static JPH::TempAllocatorImpl alloc(32 * 1024 * 1024);
			static JPH::JobSystemThreadPool threadPool(JPH::cMaxPhysicsJobs,
				JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

			// Step the world
			m_physics_system->Update(phDeltaTime, collSteps, &alloc, &threadPool);
		}

	}

	void physics_core::init()
	{
		VERB("physics_core::init");

		JPH::Trace = TraceJPHCallback;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertJPHCallback;)


		JPH::RegisterDefaultAllocator();
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

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

		//JPH::Factory::sInstance->Clear();
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