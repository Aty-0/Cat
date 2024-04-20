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
    class physical_body;
    class CATENGINE_API transform : public component
    {
    public:
        friend physical_body;
        friend physics::physics_core;

        transform();
        ~transform();

        void                reset();

        void                setPosition(glm::vec3 pos);
        void                setVelocity(glm::vec3 vel);
        void                setRotation(glm::vec3 rot);
        void                setScale(glm::vec3 scale);
        void                setScaleFactor(glm::vec3 scale);
        void                setParent(game_object* go);

        [[nodiscard]] inline glm::vec3    getPosition()  const;
        [[nodiscard]] inline glm::vec3    getRotation()  const;
        [[nodiscard]] inline glm::vec3    getVelocity()  const;
        [[nodiscard]] inline glm::vec3    getScale()     const;
        [[nodiscard]] inline glm::vec3    getScaleFactor() const;
        [[nodiscard]] inline glm::mat4&   getWorldMatrix();
       
        [[nodiscard]] inline std::string toStr() const;

        [[nodiscard]] inline game_object* getChild() const;
        [[nodiscard]] inline game_object* getParent() const;

        // is current GameObject is child for setted GameObject
        [[nodiscard]] inline bool isChildOf(game_object* go) const;

        [[nodiscard]] inline glm::mat4 getTransformation();


    protected:
        void setChild(game_object* go);
        
        // For component inspector 
        void onGuiDrawCallback();

    public:
        core::callback_storage onPositionChanged;
        core::callback_storage onRotationChanged;
        core::callback_storage onScaleChanged;
        core::callback_storage onVelocityChanged;

    private:        
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_velocity;
        glm::vec3 m_scale;
        glm::vec3 m_scale_factor;
        glm::mat4 m_world_matrix;
        game_object* m_parent;
        game_object* m_child;
    };
}