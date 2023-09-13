#include "transform.h"
#include "core/utils/logger.h"
#include "game/game_object.h"

namespace cat::game::components
{
    transform::transform() :
        m_position(VEC3_ZERO),
        m_rotation(VEC3_ZERO),
        m_scale(VEC3_ONE),
        m_world_matrix(glm::mat4(1.0f))
    {

    }

    transform::~transform()
    {

    }

    glm::mat4 transform::get_matrix_transformation()
    {
        m_world_matrix = glm::mat4(1);
        return glm::translate(m_world_matrix, m_position) *  glm::rotate(m_world_matrix, glm::radians(m_rotation.x), glm::vec3(0,0,1)) * glm::scale(m_world_matrix, m_scale);
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

    void transform::reset()
    {
        m_position  = VEC3_ZERO;
        m_rotation  = VEC3_ZERO;
        m_scale     = VEC3_ONE;
        m_scale_factor = VEC3_ONE;

        onPositionChanged();
        onRotationChanged();
        onScaleChanged();
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
}