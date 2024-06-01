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

        void                setPosition(const glm::vec3& pos);
        void                setVelocity(const glm::vec3& vel);
        void                setRotation(const glm::vec3& rot);
        void                setScale(const glm::vec3& scale);
        void                setScaleFactor(const glm::vec3& scale);
        void                setParent(game_object* go);

        void                setLocalPosition(const glm::vec3& pos);
        void                setLocalVelocity(const glm::vec3& vel);
        void                setLocalRotation(const glm::vec3& rot);
        void                setLocalScale(const glm::vec3& scale);

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

        // Note: Rotation vector values automatically converted to radians
        [[nodiscard]] inline static glm::mat4 makeRotationMatrix(const glm::mat4& world, const glm::vec3& rotation); 

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
        
        glm::vec3 m_localPosition;
        glm::vec3 m_localRotation;
        glm::vec3 m_localVelocity;
        glm::vec3 m_localScale;
        
        glm::vec3 m_scale_factor; 
        glm::mat4 m_world_matrix;
        game_object* m_parent;
        game_object* m_child;
    };
}