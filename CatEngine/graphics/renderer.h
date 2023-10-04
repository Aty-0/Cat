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
	class frame_buffer;
	class shader;
	class vertex_buffer;
	class index_buffer;

	class CATENGINE_API renderer : public core::utils::singleton<renderer>
	{
	public:
		renderer();
		~renderer();

		bool init();
		void render();
		void destroy();


		void draw_elements(std::int32_t count, std::int32_t type);

		void init_post_process();
	private:
		bool init_imgui();
		
		void imgui_render();
		void imgui_new_frame();

		static void on_get_opengl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);

		core::game_window* m_window;
		core::utils::game_time* m_time;

		// TODO: Make postprocess class 
		void draw_post_process_quad();

		// Using for post procces and other 
		std::shared_ptr<frame_buffer> m_curr_frame_buff;

		// TODO: Shader array
		std::shared_ptr<shader> m_post_proc_shader;
		std::shared_ptr<vertex_buffer> m_post_proc_vb;
		std::shared_ptr<index_buffer>  m_post_proc_ib;

	};
}

