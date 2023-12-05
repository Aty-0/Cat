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

		// run engine 
		inline bool run();
		// turn off engine 
		void destroy();

	private:
		inline bool init_GLFW();
		inline bool load_all();

		void on_loop();
		void on_update(float delta_time);

		core::input_manager*  m_input;
		core::game_window*	  m_window;
		graphics::renderer*   m_renderer;
		utils::game_time*	  m_time;
		io::resource_manager* m_rm;
		game::scene::scene_manager* m_sm;

		callback_storage*	m_on_global_update;

	// Mostly needed for script core
	public:
		[[nodiscard]] inline callback_storage* get_on_global_update() const;
	};	
}
