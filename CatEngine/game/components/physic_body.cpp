#include "physic_body.h"
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
	physic_body::physic_body()
	{
		onCreate.add(std::bind(&physic_body::createBody, this));
	}

	physic_body::~physic_body()
	{
		// TODO: Cleanup
	}
	
	void physic_body::addPosCallback()
	{
		static const auto pc = physics::physics_core::get_instance();
		const auto transform = get_owner()->get_transform();
		pc->getBodyInterface()->SetPosition(m_id, { transform->m_position.x, transform->m_position.y, transform->m_position.z }, JPH::EActivation::Activate);

		pc->getBodyInterface()->SetLinearVelocity(m_id, JPH::Vec3(0.0f, -0.003f, 0.0f));
	}

	void physic_body::createBody()
	{
		VERB("physic_body::createBody -> name: %s", get_owner()->get_name().c_str());

		static const auto pc = physics::physics_core::get_instance();
		const auto transform = get_owner()->get_transform();

		// TODO: other shape types
		JPH::BoxShapeSettings box_shape_settings(JPH::Vec3(transform->m_scale.x, 
			transform->m_scale.y, transform->m_scale.z));

		JPH::ShapeSettings::ShapeResult box_shape_result = box_shape_settings.Create();
		JPH::ShapeRefC box_shape = box_shape_result.Get();
		
		// TODO: Dynamic
		JPH::BodyCreationSettings box_creation_settings(box_shape,
			JPH::RVec3(transform->m_position.x, transform->m_position.y, 
				transform->m_position.z), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, physics::layers::MOVING); 
		m_body = pc->getBodyInterface()->CreateBody(box_creation_settings);
		CAT_ASSERT(m_body != nullptr);

		m_id = m_body->GetID();
		pc->getBodyInterface()->AddBody(m_id, JPH::EActivation::Activate);

		transform->onPositionChanged.add(std::bind(&physic_body::addPosCallback, this));
		
		pc->getBodyInterface()->SetLinearVelocity(m_id, JPH::Vec3(0.0f, -0.003f, 0.0f));
		//body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));	
	}

	[[nodiscard]] inline JPH::BodyID physic_body::getBodyId() const
	{
		return m_id;
	}
}