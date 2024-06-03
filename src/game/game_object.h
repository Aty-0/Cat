#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "game/components/transform.h"


#define CAT_UUID_REGENERATE "R"
#define CAT_DEFAULT_GAMEOBJECT_NAME "GameObject"
#define CAT_DEFAULT_GAMEOBJECT_TYPE "Default"
#define	CAT_ENGINE_GAMEOBJECT_TYPE "_Engine_"
#define CAT_DEFAULT_GAMEOBJECT_PREFIX 0

namespace cat::graphics
{
	class renderer;
}

namespace cat::game
{
	namespace scene
	{
		class scene_manager;
	}
	class serialization_game_object_impl;
	class CATENGINE_API game_object
	{
	public:
		using components_list = std::vector<std::pair<uuids::uuid, std::shared_ptr<components::component>>>;
		
		friend scene::scene_manager;
		friend serialization_game_object_impl;

		game_object();
		explicit game_object(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());
		virtual ~game_object();
	
		void render(graphics::renderer* renderer);
		void update(float deltaTime);
		void setName(std::string name);
		void setPrefix(std::int32_t prefix);
		void setType(std::string type);
		void setVisible(bool visible);
		void setEnabled(bool enabled);

		[[nodiscard]] inline std::string  getName()   const;
		[[nodiscard]] inline std::int32_t getPrefix() const;
		[[nodiscard]] inline std::string  getType()   const;
		[[nodiscard]] inline bool	isVisible() const;
		[[nodiscard]] inline bool	isEnabled() const;

		[[nodiscard]] inline components::transform* getTransform() const;
		[[nodiscard]] inline components_list getComponents() const;
		[[nodiscard]] inline core::uuid_object getUUID() const;

	private:
		std::string m_name;
		std::int32_t m_prefix;
		std::string m_type;
		bool m_enabled;
		bool m_visible;
		bool m_select;
		components::transform* m_transform;
		core::uuid_object m_uuid;

	// TODO: Cleanup that mess 
	public:
		template<typename T>
		decltype(auto) addComponent(std::shared_ptr<T> component, std::uint32_t priority = 0);

		template<typename T>
		decltype(auto) createComponent(std::uint32_t priority = 0);

		template<typename T>
		std::shared_ptr<T> initComponent(std::shared_ptr<T> component);

		// TODO: delete by priority ?
		template<typename T>
		void deleteComponent();

		template<typename T>
		[[nodiscard]] T* getComponent();

		template<typename T>
		[[nodiscard]] T* getComponentInChildren();

		template<typename T>
		[[nodiscard]] T* getComponentInParent();

	private:
		components_list m_components;

	public:
		core::callback_storage onCreate;
		core::callback_storage onDestroy;

	};

	class serialization_game_object_impl
	{
		public:
    		rfl::Field<"Name", std::string> name;
    		rfl::Field<"UUID", std::string> uuid;
    		rfl::Field<"Type", std::string> type;
    		rfl::Field<"Prefix", std::int32_t> prefix;
    		rfl::Field<"Enabled", bool> enabled;
    		rfl::Field<"Visible", bool> visible;
    		rfl::Field<"Select", bool> 	select;
			rfl::Field<"Components", std::vector<std::shared_ptr<game::components::component>>> components;

		    static serialization_game_object_impl from_class(const game::game_object& go) noexcept
			{
				std::vector<std::shared_ptr<game::components::component>> components;
				for (const auto component : go.m_components) 
				{
					components.push_back(component.second);
				}

    		    return serialization_game_object_impl {
					.name = go.m_name,
					.uuid = go.m_uuid.getIDStr(),
					.type = go.m_type,
					.prefix = go.m_prefix,
					.enabled = go.m_enabled,
					.visible = go.m_visible,
					.select = go.m_select,
					.components = components
				};
    		}		

    		game::game_object to_class() const 
			{ 
				auto go = game::game_object(); 
				game::game_object::components_list components_list;
				
				for (const auto component : components.get())
				{
					components_list.push_back({component->getUUID().getID(), component});
					component->setOwner(&go);
				}

				go.m_name = name(); 
				go.m_uuid.set(uuid()); 
				go.m_type    = type(); 
				go.m_prefix	 = prefix(); 
				go.m_enabled = enabled(); 
				go.m_visible = visible(); 
				go.m_select	 = select(); 
				go.m_components = components_list;

				return go;
			}
	};
}

namespace rfl::parsing 
{
	template <class ReaderType, class WriterType>
	struct Parser<ReaderType, WriterType, cat::game::game_object>
	    : public CustomParser<ReaderType, WriterType, cat::game::game_object,
	                          cat::game::serialization_game_object_impl> {};
}  


#include "game/game_object_inline.h"