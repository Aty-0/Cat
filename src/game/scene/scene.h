#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "game/game_object.h"

namespace cat::game::scene
{
	using scene_go_storage = std::vector<std::pair<std::pair<std::string, uuids::uuid>,
		std::shared_ptr<game::game_object>>>;
	class scene_manager;
	class serialization_scene_impl; // First we are declare class to make it friend for a scene  
	class CATENGINE_API scene
	{
	public:
		friend scene_manager;
		friend serialization_scene_impl;

		scene();
		explicit scene(const std::string& name, const scene_go_storage& storage);

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
	
	class serialization_scene_impl
	{
		public:
			// A scene name 
    		rfl::Field<"name", std::string> name;
			// Simplefied version of storage
			rfl::Field<"storage", std::vector<std::shared_ptr<game::game_object>>> storage;

		    static serialization_scene_impl from_class(const scene& scene) noexcept
			{
				// We are saving objects into a basic array of game objects
				std::vector<std::shared_ptr<game::game_object>> storage;
				for (const auto go : scene.m_storage) 
				{
					storage.push_back(go.second);
				}

    		    return serialization_scene_impl {
					.name = scene.m_name,
					.storage = storage				
				};
    		}		

    		scene to_class() const 
			{
				// Make a new scene storage for scene 
				scene_go_storage scene_storage;
				for (const auto go : storage.get())
				{
					scene_storage.push_back({{go->getName(), go->getUUID().getID()}, go});
				}
				return scene(name(), scene_storage); 
			}
	};
}

// Adding a custom parser of scene 
namespace rfl::parsing 
{
	template <class ReaderType, class WriterType>
	struct Parser<ReaderType, WriterType, cat::game::scene::scene>
	    : public CustomParser<ReaderType, WriterType, cat::game::scene::scene,
	                          cat::game::scene::serialization_scene_impl> { };
}  
