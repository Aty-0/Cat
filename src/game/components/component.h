#pragma once
#include "core/common.h"
#include "core/uuid_object.h"
#include "core/callback_storage.h"

namespace cat::game
{
	class game_object;
	class serialization_game_object_impl;
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
	class serialization_component_impl;
	
	class CATENGINE_API component
	{
		friend game_object;
		friend scene::scene_manager;
		friend serialization_component_impl;
		friend serialization_game_object_impl;

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

	class serialization_component_impl
	{
		public:
    		rfl::Field<"Name", std::string> name;
    		rfl::Field<"Selected", bool> selected;
    		rfl::Field<"UUID", std::string> uuid;

		    static serialization_component_impl from_class(const game::components::component& component) noexcept
			{
    		    return serialization_component_impl {
					.name = component.m_name,
					.selected = component.m_select,
					.uuid = component.m_uuid.getIDStr(),
				};
    		}		

    		game::components::component to_class() const 
			{ 
				auto component = game::components::component(); 
				
				component.m_name = name(); 
				component.m_uuid.set(uuid()); 
				component.m_select = selected(); 

								
				return component;
			}
	};
}



namespace rfl::parsing 
{
	template <class ReaderType, class WriterType>
	struct Parser<ReaderType, WriterType, cat::game::components::component>
	    : public CustomParser<ReaderType, WriterType, cat::game::components::component,
	                          cat::game::components::serialization_component_impl> {};
}  