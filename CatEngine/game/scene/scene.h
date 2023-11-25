#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "game/game_object.h"

namespace cat::game::scene
{
	using scene_go_storage = std::vector<std::pair<std::pair<std::string, uuids::uuid>,
		std::shared_ptr<game::game_object>>>;

	class CATENGINE_API scene
	{
	public:
		scene();
		~scene();

		// Rename object (by name)
		void rename(std::string object_name, std::string new_object_name);
		// Rename object (by uuid)
		void rename(uuids::uuid object_id, std::string new_object_name);

		// Delete game object (by name)
		void del(std::string object_name);
		// Delete game object (by uuid)
		void del(uuids::uuid object_id);

		// Replace game object (by name)
		void replace(std::string object_name, game::game_object* object);
		// Replace game object (by uuid)
		void replace(uuids::uuid object_id, game::game_object* object);

		// Clear scene 
		// NOTE: Objects with engine prefix (it is -1) can't be removed
		void clear();

		// Destroy scene, remove all objects
		void destroy();

		// Check name on uniqueness, if we found object with same name we are add num prefix 
		std::string	make_name_unique(std::string name);

		[[nodiscard]] inline scene_go_storage& get_storage();
		[[nodiscard]] inline std::string get_name() const;

		// Add game object to scene
		template<typename T = game::game_object>
		T* add(T* g);

		// Clone game object (by name)
		template<typename T = game::game_object>
		T* clone(std::string object_name, std::string new_name = std::string());

		// Clone game object (by uuid)
		template<typename T = game::game_object>
		T* clone(uuids::uuid object_id, std::string new_name = std::string());


	private:
		scene_go_storage m_storage;
		std::string m_name;
	};
}
#include "game/scene/scene_inline.h"
