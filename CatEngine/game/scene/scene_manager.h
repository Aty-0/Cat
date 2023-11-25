#pragma once
#include "core/common.h"
#include "game/scene/scene.h"

namespace cat::graphics
{
	class renderer;
}

namespace cat::game
{
	class game_object;
}

namespace cat::game::scene
{
	class CATENGINE_API scene_manager : public core::utils::singleton<scene_manager>
	{
	public:
		scene_manager();
		~scene_manager();

		// Create new scene 
		void create();
		// Load scene from file 
		bool load(std::string name);
		// Save current scene
		bool save();
		// Render game objects 
		void render(graphics::renderer* renderer);
		// Update game objects 
		void update(float DeltaTime);
		// Destroy current scene 
		void clear();
		
		void debug_render_imgui_window();

		[[nodiscard]] inline scene* get_scene() const;

		// Create game object in scene
		template<typename T = game::game_object>
		T* create_game_object(
			std::string name = std::string(),
			std::string type = std::string(),
			std::int32_t prefix = 0,
			glm::vec3 position = VEC3_ZERO,
			glm::vec3 rotation = VEC3_ZERO,
			glm::vec3 scale = VEC3_ONE);

		// Get game object from scene by name
		template<typename T = game::game_object>
		T* get_game_object_name(std::string name);

		// Get game object from scene by uuid
		template<typename T = game::game_object>
		T* get_game_object_uuid(uuids::uuid uuid);

		// Create game object function without initialization 
		template<typename T = game::game_object>
		T* create_game_object_ninit();
	private:
		scene* m_scene;
	};
}

#include "game/scene/scene_manager_inline.h"