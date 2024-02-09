#include "transform.h"
#include "core/utils/logger.h"
#include "game/game_object.h"

namespace cat::game::components
{
    transform::transform() :
        m_position(VEC3_ZERO),
        m_rotation(VEC3_ZERO),
        m_velocity(VEC3_ZERO),
        m_scale(VEC3_ONE),
        m_scale_factor(VEC3_ONE),
        m_world_matrix(glm::mat4(1.0f))
    {

    }

    transform::~transform()
    {

    }

    glm::mat4 transform::get_matrix_transformation()
    {
        m_world_matrix = glm::mat4(1);
        auto pos = VEC3_ZERO;
        auto rot = VEC3_ZERO;
        auto scale = VEC3_ONE;
        if (m_parent)
        {
            auto transform = m_parent->get_transform();
            pos = transform->m_position;
            rot = transform->m_rotation;
            scale = transform->m_scale;
        }

        const auto rotation_matrix = glm::rotate(m_world_matrix, glm::radians(m_rotation.z + rot.z), glm::vec3(0, 0, 1))
            * glm::rotate(m_world_matrix, glm::radians(m_rotation.y + rot.y), glm::vec3(0, 1, 0))
            * glm::rotate(m_world_matrix, glm::radians(m_rotation.x + rot.x), glm::vec3(1, 0, 0));

        return glm::translate(m_world_matrix, m_position + pos) * rotation_matrix * glm::scale(m_world_matrix, m_scale * m_scale_factor * scale);
    }

    void transform::set_position(glm::vec3 pos)
    {
        m_position = pos;
        onPositionChanged();
    }

    void transform::set_rotation(glm::vec3 rot)
    {
        m_rotation = rot;
        onRotationChanged();
    }

    void transform::set_scale(glm::vec3 scale)
    {
        m_scale = scale;
        onScaleChanged();
    }

    void transform::set_scale_factor(glm::vec3 scale)
    {
        m_scale_factor = scale;
    }

    void transform::reset()
    {
        m_position  = VEC3_ZERO;
        m_rotation  = VEC3_ZERO;
        m_scale     = VEC3_ONE;
        m_scale_factor = VEC3_ONE;

        onPositionChanged();
        onRotationChanged();
        onScaleChanged();
        onVelocityChanged();
    }


    bool transform::is_child_of(game_object* go) const
    {
        if (m_child == nullptr)
        {
            ERR("Child is empty, so what's we need to check ?");
            return false;
        }

        return m_child == go->get_transform()->get_child();
    }

    void transform::set_child(game_object* go)
    {
        if (get_owner() == go)
        {
            ERR("You can't assign yourself as child!");
            return;
        }

        m_child = go;
    }

    void transform::set_parent(game_object* go)
    {
        if (get_owner() == go)
        {
            ERR("You can't assign yourself as parent!");
            return;
        }

        go->get_transform()->set_child(get_owner());
        m_parent = go;
    }

    glm::vec3 transform::get_scale_factor()   const
    {
        return m_scale_factor;
    }

    glm::vec3 transform::get_position()  const
    {
        return m_position;
    }

    glm::vec3 transform::get_rotation()  const
    {
        return m_rotation;
    }

    glm::vec3 transform::get_scale()     const
    {
        return m_scale;
    }

    glm::vec3 transform::get_velocity()     const
    {
        return m_velocity;
    }

    glm::mat4& transform::get_world_matrix()
    {
        return m_world_matrix;
    }

    game_object* transform::get_child() const
    {
        return m_child;
    }

    game_object* transform::get_parent() const
    {
        return m_parent;
    }

    void transform::set_velocity(glm::vec3 vel)
    {
        m_velocity = vel;
        onVelocityChanged();
    }

    const char* transform::to_string() const
    {
        auto text = core::utils::to_str("pos: %f %f %f\nrot: %f %f %f\nscale: %f %f %f\nvel: %f %f %f\nsc_factor: %f %f %f",
            m_position.x, m_position.y, m_position.z,
            m_rotation.x, m_rotation.y, m_rotation.z,
            m_scale.x, m_scale.y, m_scale.z,
            m_velocity.x, m_velocity.y, m_velocity.z,
            m_scale_factor.x, m_scale_factor.y, m_scale_factor.z
        );

        return text;
    }

}