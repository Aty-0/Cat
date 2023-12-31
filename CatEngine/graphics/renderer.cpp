#include "renderer.h"

#include "core/callback_storage.h"
#include "core/utils/logger.h"
#include "core/game_window.h"
#include "core/utils/game_time.h"

#include "game/scene/scene_manager.h"
#include "game/components/camera.h"

#include "Libs/imgui/imgui_impl_opengl3.h"
#include "Libs/imgui/imgui_impl_glfw.h"

#include "graphics/frame_buffer.h"
#include "graphics/vertex_buffer.h"
#include "graphics/index_buffer.h"
#include "graphics/vertex.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

namespace cat::graphics
{
	core::callback_storage renderer::onImGuiRender;

	renderer::renderer() : 
		m_renderImgui(true), 
		m_curr_frame_buff(nullptr),
		m_disable_post_proc(false),
		m_post_proc_ib(nullptr),
		m_post_proc_shader(nullptr),
		m_post_proc_vb(nullptr),
		m_time(nullptr),
		m_window(nullptr)
	{

	}

	renderer::~renderer()
	{

	}

	void renderer::toggle_imgui_rendering()
	{
		m_renderImgui =! m_renderImgui;
	}

	void renderer::destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void renderer::debug_print_existing_ext()
	{
		std::int32_t ext_num = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &ext_num);
		
		for (std::int32_t i = 0; i < ext_num; i++)
		{
			const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			INFO("%i Extension %s", i, extension);
		}
	}

	bool renderer::init()
	{
		INFO("Renderer initialize...");

		const auto res = glewInit();
		if (res != GLEW_OK)
		{
			ERR("Error: '%s'", glewGetErrorString(res));
			return false;
		}

		//debug_print_existing_ext();

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(on_get_opengl_error, nullptr);
		
		if (!init_imgui())
			return false;
		
		m_time = core::utils::game_time::get_instance();

		onImGuiRender.add(std::bind(&renderer::render_debug_imgui_window, this));
		return true;
	}

	void renderer::on_get_opengl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		const auto source_str = [source]() {
			switch (source)
			{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY:  return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			default: return "UNKNOWN";
			}
		}();

		const auto type_str = [type]() {
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER:  return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			default: return "UNKNOWN";
			}
		}();

		const auto severity_str = [severity]() {
			switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "UNKNOWN";
			}
		}();

		static const bool verbose = false;
		
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			ERR("GL: %s %s %s %s", source_str, type_str, severity_str, message);
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			WARN("GL: %s %s %s %s", source_str, type_str, severity_str, message);
			break;
		default:
			if(verbose)
				VERB("GL: %s %s %s %s", source_str, type_str, severity_str, message);
			break;
		}

	}

	void renderer::render()
	{
		if (m_window->get_width() == 0 
			|| m_window->get_height() == 0)
			return;

		imgui_new_frame();

		if (m_disable_post_proc)
		{
			m_curr_frame_buff->bind();
		}

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static const auto sm = game::scene::scene_manager::get_instance();
		sm->render(this);

		// TODO: UI rendering will be located there 
	
		glDisable(GL_DEPTH_TEST);

		if (m_disable_post_proc)
		{
			m_curr_frame_buff->unbind_buffer();
			
			// Clear all relevant buffers
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			draw_post_process_quad();
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool renderer::init_imgui()
	{
		INFO("Initialize Imgui...");
		m_window = core::game_window::get_instance();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		
		if (!ImGui_ImplGlfw_InitForOpenGL(m_window->get_GLFW_window(), true))
		{
			FATAL("ImGui For OpenGL Impl stage failed");
			return false;
		}

		if (!ImGui_ImplOpenGL3_Init())
		{
			FATAL("ImGui OpenGL stage failed");
			return false;
		}

		// Apply style
		ImGui::StyleColorsDark();

		return true;
	}
	
	void renderer::render_debug_imgui_window()
	{
		ImGui::Begin("Debug window");

		ImGui::Text("FPS %d", m_time->get_fps());
		ImGui::Text("Time %f", m_time->get_time());
		ImGui::Text("Delta time %f", m_time->get_delta_time());
		if (ImGui::Checkbox("Disable Post Process", &m_disable_post_proc))
		{
			recreate_post_process();
		}

		ImGui::End();
	}

	void renderer::imgui_render()
	{
		onImGuiRender.run_all();		
	}

	void renderer::imgui_new_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (m_renderImgui)
		{
			imgui_render();
		}

		ImGui::Render();
	}


	void renderer::draw_elements(std::int32_t count, std::int32_t type)
	{
		glDrawElements(type, count, GL_UNSIGNED_INT, 0);
	}

	void renderer::init_post_process()
	{
		m_curr_frame_buff = std::make_shared<frame_buffer>(*new frame_buffer());
		m_curr_frame_buff->gen();

		std::vector<graphics::vertex> vb_data = { { glm::vec3(1.0f,  1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }, 
												{ glm::vec3(1.0f, -1.0f, 0.0f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }, 
												{ glm::vec3(-1.0f, -1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 0.0f) },
												{ glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 1.0f) } 
		};

		m_post_proc_vb = std::make_shared<vertex_buffer>(*new vertex_buffer());

		m_post_proc_vb->gen();
		m_post_proc_vb->set_buffer_data<graphics::vertex>(vb_data, GL_STATIC_DRAW);

		std::vector<std::uint32_t> ib_data =
		{
			0, 1, 3, 
			1, 2, 3  
		};

		m_post_proc_ib = std::make_shared<index_buffer>(*new index_buffer());
		m_post_proc_ib->gen();
		m_post_proc_ib->set_buffer_data<std::uint32_t>(ib_data, GL_STATIC_DRAW);

		m_post_proc_vb->set_attrib(0, static_cast<std::uint32_t>(ib_data.size() / 2), GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, pos)));
		m_post_proc_vb->set_attrib(1, static_cast<std::uint32_t>(ib_data.size() / 2), GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, color)));
		m_post_proc_vb->set_attrib(2, static_cast<std::uint32_t>(ib_data.size() / 2), GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, uv)));

		m_post_proc_vb->unbind_all();

		m_post_proc_shader = std::make_shared<graphics::shader>(*new graphics::shader());
		CAT_ASSERT(m_post_proc_shader->load("postprocess"));

		core::game_window::onWindowResized.add(std::bind(&graphics::renderer::recreate_post_process, this));
	}

	void renderer::recreate_post_process()
	{
		//VERB("renderer::recreate_post_process");		
		if (m_window->get_width() == 0
			|| m_window->get_height() == 0 
			|| !m_disable_post_proc
			|| !m_window->is_resized())
			return;
		
		// Remove old and create new one 
		m_curr_frame_buff->clear();
		m_curr_frame_buff->gen();
	}
	
	void renderer::draw_post_process_quad()
	{
		m_post_proc_shader->bind();
		m_post_proc_vb->bind();

		m_curr_frame_buff->get_texture()->bind();
		draw_elements(6, GL_TRIANGLES);

		m_curr_frame_buff->get_texture()->unbind();
	}
}