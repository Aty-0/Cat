#include "physics_core.h"
#include "core/utils/logger.h"

#include "game/game_object.h"
#include "game/components/physic_body.h"

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
		const auto phbody = go->get_component<game::components::physic_body>();
		if (phbody == nullptr)
			return;

		const auto id = phbody->getBodyId();

		//m_physics_system->OptimizeBroadPhase();
		std::int32_t step = 0;
		if (m_body_interface->IsActive(id))
		{			
			step++;
			// Output current position and velocity of the sphere
			JPH::RVec3 position = m_body_interface->GetCenterOfMassPosition(id);
			JPH::Vec3 velocity = m_body_interface->GetLinearVelocity(id);
			go->get_transform()->m_position = { position.GetX(), position.GetY(), position.GetZ() };
			
			//VERB("step %i posx: %f posy: %f posz: %f velx: %f vely: %f velz: %f", step,
			//	position.GetX(), position.GetY(), position.GetZ(),
			//	velocity.GetX(), velocity.GetY(), velocity.GetZ());

	
			// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. 
			// Do 1 collision step per 1 / 60th of a second (round up).
			const int cCollisionSteps = 1;
			const float cDeltaTime = 1.0f / (deltaTime * 1000000.0f);

			
			// FIXME: 
			static JPH::TempAllocatorImpl alloc(10 * 1024 * 1024);
			static JPH::JobSystemThreadPool threadPool(JPH::cMaxPhysicsJobs,
				JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
			// Step the world
			m_physics_system->Update(cDeltaTime, cCollisionSteps, &alloc, &threadPool);
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