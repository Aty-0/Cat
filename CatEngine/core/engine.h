#pragma once
#include "core/common.h"

namespace cat
{	
	namespace io
	{
		class resource_manager;
	}

	namespace game::scene
	{
		class scene_manager;
	}

	namespace graphics
	{
		class renderer;
	}

	namespace physics
	{
		class physics_core;
	}
}


namespace cat::core
{	
	class input_manager;
	class game_window;
	class callback_storage;
	
	namespace utils
	{
		class game_time;
	}

	class CATENGINE_API engine 
		: public cat::core::utils::singleton<engine>
	{
	public:
		engine();
		~engine();

		bool run();
		void destroy();

	private:		
		inline void onLoop();
		inline void onUpdate(float delta_time);
		inline bool initGLFW();

		core::input_manager*  m_input;
		core::game_window*	  m_window;
		graphics::renderer*   m_renderer;
		utils::game_time*	  m_time;
		io::resource_manager* m_rm;
		game::scene::scene_manager* m_sm;
		physics::physics_core* m_physics;

	public:
		// Mostly needed for script core
		// If we are want to add something to global update
		static callback_storage* onGlobalUpdate;
	};	
}
