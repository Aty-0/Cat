#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "game/game_object.h"

namespace cat::game::scene
{
	using scene_go_storage = std::vector<std::pair<std::pair<std::string, uuids::uuid>,
		std::shared_ptr<game::game_object>>>;
	class scene_manager;
	class CATENGINE_API scene
	{
	public:
		friend scene_manager;

		scene();
		~scene();

		// NOTE: Game object with a engine prefix will be not erased 
		void clear();

		// Destroy scene, remove all from scene 
		void destroy();

		[[nodiscard]] inline scene_go_storage& getStorage();
		[[nodiscard]] inline std::string getName() const;

	private:
		scene_go_storage m_storage;
		std::string m_name;
	};
}
