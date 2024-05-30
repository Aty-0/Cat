#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "core/callback_storage.h"

namespace cat::game
{
	class game_object;
	namespace scene
	{
		class scene_manager;
	}
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
		friend scene::scene_manager;
	public:
		component();
		virtual ~component();

		virtual void onRender(graphics::renderer* renderer) { }
		virtual void onUpdate(float deltaTime) { }

		[[nodiscard]] inline core::uuid_object getUUID() const;
		[[nodiscard]] inline std::string   getName() const;
		[[nodiscard]] inline game_object*  getOwner() const;		
	protected:
		void setOwner(game_object* go);
		void setName(const std::string& name);

	private:
		std::string			m_name;
		game_object*		m_owner;
		bool				m_select;
		core::uuid_object	m_uuid;

	public:
		core::callback_storage onCreate;
		core::callback_storage onDestroy;

		core::callback_storage onEditGuiDraw;
	};
}