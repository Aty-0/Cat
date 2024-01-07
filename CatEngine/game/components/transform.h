#pragma once
#include "core/common.h"
#include "game/components/component.h"

namespace cat::game
{
    class game_object;
}

namespace cat::physics
{
    class physics_core;
}

namespace cat::game::components
{
    class physic_body;
    class CATENGINE_API transform : public component
    {
    public:
        friend physic_body;
        friend physics::physics_core;

        transform();
        ~transform();

        void                reset();

        void                set_position(glm::vec3 pos);
        void                set_rotation(glm::vec3 rot);
        void                set_scale(glm::vec3 scale);
        void                set_scale_factor(glm::vec3 scale);
        void                set_parent(game_object* go);

        [[nodiscard]] inline glm::vec3    get_position()  const;
        [[nodiscard]] inline glm::vec3    get_rotation()  const;
        [[nodiscard]] inline glm::vec3    get_scale()     const;
        [[nodiscard]] inline glm::vec3    get_scale_factor() const;
        [[nodiscard]] inline glm::mat4&   get_world_matrix();

        [[nodiscard]] inline game_object* get_child() const;
        [[nodiscard]] inline game_object* get_parent() const;

        // is current GameObject is child for setted GameObject
        [[nodiscard]] inline bool is_child_of(game_object* go) const;

        [[nodiscard]] inline glm::mat4 get_matrix_transformation();
    protected:
        void set_child(game_object* go);

    public:
        core::callback_storage onPositionChanged;
        core::callback_storage onRotationChanged;
        core::callback_storage onScaleChanged;

    private:        
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::vec3 m_scale_factor;
        glm::mat4 m_world_matrix;
        game_object* m_parent;
        game_object* m_child;
    };
}