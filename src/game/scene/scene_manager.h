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
		
		void drawEditorSceneInspector();

		[[nodiscard]] inline scene* getScene() const;

		void rename(game::game_object* go, std::string new_name);
		void replace(game::game_object* go_first, game::game_object* go_second);
		void del(game::game_object* go);

		// Create game object in scene
		template<typename T = game::game_object>
		inline decltype(auto) createGameObject(
			std::string name = std::string(),
			std::string type = std::string(),
			std::int32_t prefix = 0,
			glm::vec3 position = VEC3_ZERO,
			glm::vec3 rotation = VEC3_ZERO,
			glm::vec3 scale = VEC3_ONE);

		// Get game object by name
		template<typename T = game::game_object>
		inline T* getGameObjectName(std::string name);

		// Get game object by uuid
		template<typename T = game::game_object>
		inline T* getGameObjectUUID(uuids::uuid uuid);

		// Create game object function without full initialization 
		template<typename T = game::game_object>
		inline decltype(auto) createGameObjectWithoutInit();

		template<typename T = game::game_object>
		decltype(auto) clone(T* go, std::string new_name = std::string());
		
		// NOTE: You can add game objects which you create manually 
		template<typename T = game::game_object>
		decltype(auto) add(T* g);

		scene_go_storage::iterator findGameObjectStr(const std::string& name);
		scene_go_storage::iterator findGameObjectUUID(const uuids::uuid& uuid);

		// Check name on uniqueness, if we found object with same name we are add num prefix 
		inline std::string makeGameObjectNameUnique(const std::string& name);
	private:
		scene* m_scene;
		game::game_object* m_selected;
	};
}

#include "game/scene/scene_manager_inline.h"