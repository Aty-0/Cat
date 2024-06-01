#include "transform.h"
#include "core/utils/logger.h"
#include "game/game_object.h"

#include "graphics/renderer.h"

namespace cat::game::components
{
    transform::transform() :
        m_position(VEC3_ZERO),
        m_rotation(VEC3_ZERO),
        m_velocity(VEC3_ZERO),
        m_scale(VEC3_ONE),        
        m_localPosition(VEC3_ZERO),
        m_localRotation(VEC3_ZERO),
        m_localVelocity(VEC3_ZERO),
        m_localScale(VEC3_ONE),
        m_scale_factor(VEC3_ONE),
        m_world_matrix(glm::mat4(1.0f))
    {
        onEditGuiDraw.add(std::bind(&transform::onGuiDrawCallback, this));
    }

    transform::~transform()
    {

    }
    
    glm::mat4 transform::makeRotationMatrix(const glm::mat4& world, const glm::vec3& rotation)
    {
        return glm::rotate(world, glm::radians(rotation.z), glm::vec3(0, 0, 1))
            * glm::rotate(world, glm::radians(rotation.y), glm::vec3(0, 1, 0))
            * glm::rotate(world, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    }

    glm::mat4 transform::getTransformation()
    {
        // TODO: So, we need to apply a local vars for child somehow 
        m_world_matrix = glm::mat4(1);
        const auto rotation_matrix = makeRotationMatrix(m_world_matrix, m_rotation);
        return glm::translate(m_world_matrix, m_position) * rotation_matrix * glm::scale(m_world_matrix, m_scale * m_scale_factor);
    }

    void transform::setPosition(const glm::vec3& pos)
    {
        m_position = pos;
        onPositionChanged();
    }

    void transform::setRotation(const glm::vec3& rot)
    {
        m_rotation = rot;
        onRotationChanged();
    }

    void transform::setScale(const glm::vec3& scale)
    {
        m_scale = scale;
        onScaleChanged();
    }

    void transform::setScaleFactor(const glm::vec3& scale)
    {
        m_scale_factor = scale;
    }

    void transform::setLocalPosition(const glm::vec3& pos)
    {
        m_localPosition = pos;
    }
    
    void transform::setLocalVelocity(const glm::vec3& vel)
    {
        m_localVelocity = vel;
    }
    
    void transform::setLocalRotation(const glm::vec3& rot)
    {
        m_localRotation = rot;
    }

    void transform::setLocalScale(const glm::vec3& scale)
    {
        m_localScale = scale;
    }

    void transform::reset()
    {
        m_position  = VEC3_ZERO;
        m_rotation  = VEC3_ZERO;
        m_scale     = VEC3_ONE;

        m_localPosition  = VEC3_ZERO;
        m_localRotation  = VEC3_ZERO;
        m_localScale     = VEC3_ONE;
        
        m_scale_factor = VEC3_ONE;

        onPositionChanged();
        onRotationChanged();
        onScaleChanged();
        onVelocityChanged();
    }


    bool transform::isChildOf(game_object* go) const
    {
        if (m_child == nullptr)
        {
            ERR("Child is empty, so what's we need to check ?");
            return false;
        }

        return m_child == go->getTransform()->getChild();
    }

    void transform::setChild(game_object* go)
    {
        if (getOwner() == go)
        {
            ERR("You can't assign yourself as child!");
            return;
        }

        m_child = go;
    }

    void transform::setParent(game_object* go)
    {
        const auto owner = getOwner();
        if (owner == go)
        {
            ERR("You can't assign yourself as parent!");
            return;
        }

        go->getTransform()->setChild(owner);
        m_parent = go;
    }

    glm::vec3 transform::getScaleFactor()   const
    {
        return m_scale_factor;
    }

    glm::vec3 transform::getPosition()  const
    {
        return m_position;
    }

    glm::vec3 transform::getRotation()  const
    {
        return m_rotation;
    }

    glm::vec3 transform::getScale()     const
    {
        return m_scale;
    }

    glm::vec3 transform::getVelocity()     const
    {
        return m_velocity;
    }

    glm::mat4& transform::getWorldMatrix()
    {
        return m_world_matrix;
    }

    game_object* transform::getChild() const
    {
        return m_child;
    }

    game_object* transform::getParent() const
    {
        return m_parent;
    }

    void transform::setVelocity(const glm::vec3& vel)
    {
        m_velocity = vel;
        onVelocityChanged();
    }

    std::string transform::toStr() const
    {
        const auto text = core::utils::toStr("pos: %f %f %f\nrot: %f %f %f\nscale: %f %f %f\nvel: %f %f %f\nsc_factor: %f %f %f",
            m_position.x, m_position.y, m_position.z,
            m_rotation.x, m_rotation.y, m_rotation.z,
            m_scale.x, m_scale.y, m_scale.z,
            m_velocity.x, m_velocity.y, m_velocity.z,
            m_scale_factor.x, m_scale_factor.y, m_scale_factor.z
        );

        return text;
    }

    void transform::onGuiDrawCallback()
    {
        float pos[3] = { m_position.x, m_position.y, m_position.z };
        if (ImGui::InputFloat3("position", pos))
        {
            m_position.x = pos[0]; 
            m_position.y = pos[1]; 
            m_position.z = pos[2];

            onPositionChanged();
        }

        float rot[3] = { m_rotation.x, m_rotation.y, m_rotation.z };
        if (ImGui::InputFloat3("rotation", rot))
        {
            m_rotation.x = rot[0];
            m_rotation.y = rot[1];
            m_rotation.z = rot[2];

            onRotationChanged();
        }

        float scale[3] = { m_scale.x, m_scale.y, m_scale.z };
        if (ImGui::InputFloat3("scale", scale))
        {
            m_scale.x = scale[0];
            m_scale.y = scale[1];
            m_scale.z = scale[2];

            onScaleChanged();
        }

        float vel[3] = { m_velocity.x, m_velocity.y, m_velocity.z };
        if (ImGui::InputFloat3("velocity", vel))
        {
            m_velocity.x = vel[0];
            m_velocity.y = vel[1];
            m_velocity.z = vel[2];

            onVelocityChanged();
        }

        if (ImGui::Button("reset"))
        {
            reset();
        }
    }

}