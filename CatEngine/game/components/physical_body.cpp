#include "physical_body.h"
#include "physics/physics_core.h"
#include "physics/layers.h"

#include "core/utils/logger.h"

#include "game/game_object.h"
#include "game/components/transform.h"

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace cat::game::components
{	
	physical_body::physical_body()
	{
		onCreate.add(std::bind(&physical_body::createBody, this));
	}

	physical_body::~physical_body()
	{
		// Remove and destroy our physic body lol
		auto pc = physics::physics_core::get_instance();
		
		pc->getBodyInterface()->RemoveBody(m_id);
		pc->getBodyInterface()->DestroyBody(m_id);

	}

	// TODO: switch Activation state
	void physical_body::updateBodyInterfaceVelocity()
	{
		static const auto pc = physics::physics_core::get_instance();
		const auto transform = get_owner()->get_transform();
		pc->getBodyInterface()->SetLinearVelocity(m_id, { transform->m_velocity.x, transform->m_velocity.y, transform->m_velocity.z });
	}

	void physical_body::updateBodyInterfacePosition()
	{
		static const auto pc = physics::physics_core::get_instance();
		const auto transform = get_owner()->get_transform();
		pc->getBodyInterface()->SetPosition(m_id, { transform->m_position.x, transform->m_position.y, transform->m_position.z },
			JPH::EActivation::Activate);
	}

	void physical_body::setMotionType(motion_type type)
	{
		static const auto pc = physics::physics_core::get_instance();
		switch (type)
		{
		case motion_type::Static:
			pc->getBodyInterface()->SetObjectLayer(m_id, physics::layers::NON_MOVING);
			break;
		case motion_type::Dynamic:
		case motion_type::Kinematic:
			pc->getBodyInterface()->SetObjectLayer(m_id, physics::layers::MOVING);
			break;
		}

		pc->getBodyInterface()->SetMotionType(m_id, type, JPH::EActivation::Activate);		
	}

	void physical_body::createBody()
	{
		VERB("physic_body::createBody -> name: %s", get_owner()->get_name().c_str());

		static const auto pc = physics::physics_core::get_instance();
		const auto transform = get_owner()->get_transform();

		// TODO: other shape types
		JPH::BoxShapeSettings box_shape_settings(JPH::Vec3(transform->m_scale.x, 
			transform->m_scale.y, transform->m_scale.z));

		JPH::ShapeSettings::ShapeResult box_shape_result = box_shape_settings.Create();
		JPH::ShapeRefC box_shape = box_shape_result.Get();		
		JPH::BodyCreationSettings box_creation_settings(box_shape,
			JPH::RVec3(transform->m_position.x, transform->m_position.y, 
				transform->m_position.z), JPH::Quat::sIdentity(), JPH::EMotionType::Static, physics::layers::NON_MOVING);
		
		// We can switch object types
		box_creation_settings.mAllowDynamicOrKinematic = true;
		
		m_body = pc->getBodyInterface()->CreateBody(box_creation_settings);
		CAT_ASSERT(m_body != nullptr);

		m_id = m_body->GetID();
		pc->getBodyInterface()->AddBody(m_id, JPH::EActivation::Activate);

		pc->getBodyInterface()->SetLinearVelocity(m_id, 
			{ transform->m_velocity.x, transform->m_velocity.y, transform->m_velocity.z });

		transform->onPositionChanged.add(std::bind(&physical_body::updateBodyInterfacePosition, this));
		transform->onVelocityChanged.add(std::bind(&physical_body::updateBodyInterfaceVelocity, this));
	}

	JPH::BodyID physical_body::getBodyId() const
	{
		return m_id;
	}
}