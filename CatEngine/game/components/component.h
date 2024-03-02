#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "core/callback_storage.h"

namespace cat::game
{
	class game_object;
}

namespace cat::graphics
{
	class renderer;
}

namespace cat::game::components
{
	class transform;

	class CATENGINE_API component
	{
		friend game_object;
	public:
		component();
		virtual ~component();

		
		virtual void on_render(graphics::renderer* renderer) { }
		virtual void on_update(float deltaTime) { }

		[[nodiscard]] inline core::uuid_object get_uuid() const;
		[[nodiscard]] inline std::string   get_name() const;
		[[nodiscard]] inline game_object*  get_owner() const;		
	protected:
		void set_owner(game_object* go);
		void set_name(std::string name);

	private:
		std::string			m_name;
		game_object*		m_owner;
		bool				m_isSelected;
		core::uuid_object	m_uuid;

	public:
		core::callback_storage onCreate;
		core::callback_storage onDestroy;

	};
}