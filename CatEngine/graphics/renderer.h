#pragma once
#include "core/common.h"

namespace cat::core
{
	class game_window;

	namespace utils
	{
		class game_time;
	}
}

namespace cat::graphics
{
	class CATENGINE_API renderer : public core::utils::singleton<renderer>
	{
	public:
		renderer();
		~renderer();

		bool init();
		void render();
		void destroy();

		void draw_elements(std::int32_t count, std::int32_t type);
	private:
		bool init_imgui();
		
		void imgui_render();
		void imgui_new_frame();

		static void on_get_opengl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);

		core::game_window* m_window;
		core::utils::game_time* m_time;

	};
}

