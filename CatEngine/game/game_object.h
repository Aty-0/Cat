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

	class CATENGINE_API game_object
	{
	public:
		using components_list = std::vector<std::pair<std::pair<uuids::uuid, std::type_index>, components::component*>>;
		
		friend scene::scene_manager;

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

	public:
		template<typename T>
		decltype(auto) addComponent(T* component, std::uint32_t priority = 0);

		template<typename T>
		decltype(auto) initComponent(T* component);

		template<typename T>
		decltype(auto) createComponent(std::uint32_t priority = 0);

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
}
#include "game/game_object_inline.h"