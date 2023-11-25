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
	using components_list = std::vector<std::pair<std::pair<uuids::uuid, std::type_index>, components::component*>>;
	class CATENGINE_API game_object : public core::uuid_object
	{
	public:
		game_object();
		explicit game_object(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());
		virtual ~game_object();

		void render(graphics::renderer* renderer);
		void update(float deltaTime);
		void set_name(std::string name);
		void set_prefix(std::int32_t prefix);
		void set_type(std::string type);
		void set_visible(bool visible);
		void set_enabled(bool enabled);

		[[nodiscard]] inline std::string  get_name()   const;
		[[nodiscard]] inline std::int32_t get_prefix() const;
		[[nodiscard]] inline std::string  get_type()   const;
		[[nodiscard]] inline bool	is_visible() const;
		[[nodiscard]] inline bool	is_enabled() const;

		[[nodiscard]] inline components::transform* get_transform() const;
		[[nodiscard]] inline components_list get_components() const;

	private:
		std::string m_name;
		std::int32_t m_prefix;
		std::string m_type;
		bool m_enabled;
		bool m_visible;
		components::transform* m_transform;
		
	public:
		template<typename T>
		T* add_component(T* component, std::uint32_t priority = 0);

		template<typename T>
		T* init_component(T* component);

		// TODO: delete by priority ?
		template<typename T>
		void delete_component();

		template<typename T>
		T* create_component(std::uint32_t priority = 0);

		template<typename T>
		[[nodiscard]] T* get_component();

		template<typename T>
		[[nodiscard]] T* get_component_in_children();

		template<typename T>
		[[nodiscard]] T* get_component_in_parent();

	private:
		components_list m_components;

	public:
		core::callback_storage onCreate;
		core::callback_storage onDestroy;
	};
}
#include "game/game_object_inline.h"