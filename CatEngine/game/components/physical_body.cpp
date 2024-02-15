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
		static const auto pc = physics::physics_core::get_instance();
		m_core = pc;
		onCreate.add(std::bind(&physical_body::createBody, this));
	}

	physical_body::~physical_body()
	{
		// Remove and destroy our physical body lol
		m_core->getBodyInterface()->RemoveBody(m_id);
		m_core->getBodyInterface()->DestroyBody(m_id);
	}

	// TODO: switch Activation state
	void physical_body::updateBodyInterfaceVelocity()
	{
		const auto transform = get_owner()->get_transform();
		m_core->getBodyInterface()->SetLinearVelocity(m_id, { transform->m_velocity.x, transform->m_velocity.y, transform->m_velocity.z });
	}

	void physical_body::updateBodyInterfacePosition()
	{
		const auto transform = get_owner()->get_transform();
		m_core->getBodyInterface()->SetPosition(m_id, { transform->m_position.x, transform->m_position.y, transform->m_position.z },
			JPH::EActivation::Activate);
	}
	
	void physical_body::setMass(float mass)
	{
		if (!m_body->IsStatic())
		{
			auto prop = m_body->GetMotionProperties();
			JPH::MassProperties mass_prop;
			mass_prop.ScaleToMass(mass);
			mass_prop.mInertia(3, 3) = 1.0f;
			prop->SetMassProperties(prop->GetAllowedDOFs(), mass_prop);
		}
	}

	void physical_body::setShape(JPH::Ref<JPH::Shape> shape)
	{
		// TODO: Get from body ? | bool, JPH::EActivation

		m_core->getBodyInterface()->SetShape(m_id, shape, false, JPH::EActivation::Activate);
	}

	void physical_body::setMotionQuality(motion_quality quality)
	{	
		m_core->getBodyInterface()->SetMotionQuality(m_id, quality);
	}

	void physical_body::setFriction(float friction)
	{
		m_core->getBodyInterface()->SetFriction(m_id, friction);
	}

	void physical_body::setGravityFactor(float factor)
	{
		m_core->getBodyInterface()->SetGravityFactor(m_id, factor);
	}

	void physical_body::setMotionType(motion_type type)
	{
		switch (type)
		{
		case motion_type::Static:
			m_core->getBodyInterface()->SetObjectLayer(m_id, physics::layers::NON_MOVING);
			break;
		case motion_type::Dynamic:
		case motion_type::Kinematic:
			m_core->getBodyInterface()->SetObjectLayer(m_id, physics::layers::MOVING);
			break;
		}

		m_core->getBodyInterface()->SetMotionType(m_id, type, JPH::EActivation::Activate);
	}

	void physical_body::createBody()
	{
		//VERB("physic_body::createBody -> name: %s", get_owner()->get_name().c_str());
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
		
		m_body = m_core->getBodyInterface()->CreateBody(box_creation_settings);
		CAT_ASSERT(m_body != nullptr);

		m_id = m_body->GetID();
		m_core->getBodyInterface()->AddBody(m_id, JPH::EActivation::Activate);

		m_core->getBodyInterface()->SetLinearVelocity(m_id,
			{ transform->m_velocity.x, transform->m_velocity.y, transform->m_velocity.z });

		transform->onPositionChanged.add(std::bind(&physical_body::updateBodyInterfacePosition, this));
		transform->onVelocityChanged.add(std::bind(&physical_body::updateBodyInterfaceVelocity, this));
	}

	JPH::BodyID physical_body::getBodyId() const
	{
		return m_id;
	}
}