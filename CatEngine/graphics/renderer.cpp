#include "renderer.h"
#include "core/utils/logger.h"
#include "core/game_window.h"
#include "core/utils/game_time.h"
#include "game/scene/scene_manager.h"

#include "game/components/camera.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_opengl3.h"
#include "Libs/imgui/imgui_impl_glfw.h"

namespace cat::graphics
{
	renderer::renderer()
	{

	}

	renderer::~renderer()
	{

	}

	void renderer::destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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

		/*
		std::int32_t ext_num = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &ext_num);

		for (std::int32_t i = 0; i < ext_num; i++)
		{
			const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
			INFO("%i Extension %s", i, extension);
		}
		*/

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(on_get_opengl_error, nullptr);
		
		if (!init_imgui())
			return false;
		
		m_time = core::utils::game_time::get_instance();

		return true;
	}

	void renderer::on_get_opengl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		auto source_str = [source]() {
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

		auto type_str = [type]() {
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

		auto severity_str = [severity]() {
			switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "UNKNOWN";
			}
		}();

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
			VERB("GL: %s %s %s %s", source_str, type_str, severity_str, message);
			break;
		}

	}

	void renderer::render()
	{
		imgui_new_frame();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static const auto sm = game::scene::scene_manager::get_instance();
		sm->render(this);

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

	void renderer::imgui_render()
	{
		/*static const auto sm = game::scene::scene_manager::get_instance();
		auto test = sm->get_game_object_name<>("SomeTests");
		auto test_tr = test->get_transform();
		ImGui::Begin("Transform");
		ImGui::Text("Position %f %f %f", test_tr->get_position().x, test_tr->get_position().y, test_tr->get_position().z);
		ImGui::Text("Rotation %f %f %f", test_tr->get_rotation().x, test_tr->get_rotation().y, test_tr->get_rotation().z);
		ImGui::Text("Scale %f %f %f", test_tr->get_scale().x, test_tr->get_scale().y, test_tr->get_scale().z);
		ImGui::Text("Matrix:");

		auto matrix = test_tr->get_world_matrix();
		ImGui::Text("%f %f %f", matrix[0][0], matrix[0][1], matrix[0][2]);
		ImGui::Text("%f %f %f", matrix[1][0], matrix[1][1], matrix[1][2]);
		ImGui::Text("%f %f %f", matrix[2][0], matrix[2][1], matrix[2][2]);


		ImGui::Text("camera:");
		auto camera = sm->get_game_object_name<>(game::components::camera::EngineCameraName);
		auto cam_tr = camera->get_transform();
		ImGui::Text("Position %f %f %f", cam_tr->get_position().x, cam_tr->get_position().y, cam_tr->get_position().z);
		ImGui::Text("Rotation %f %f %f", cam_tr->get_rotation().x, cam_tr->get_rotation().y, cam_tr->get_rotation().z);
		ImGui::Text("Scale %f %f %f", cam_tr->get_scale().x, cam_tr->get_scale().y, cam_tr->get_scale().z);

		ImGui::End();
		*/

		ImGui::Begin("Debug window");
		
		ImGui::Text("FPS %d",	m_time->get_fps());
		ImGui::Text("Time %f",	m_time->get_time());
		ImGui::Text("Delta time %f", m_time->get_delta_time());

		ImGui::End();

	}

	void renderer::imgui_new_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		imgui_render();
		ImGui::Render();
	}


	void renderer::draw_elements(std::int32_t count, std::int32_t type)
	{
		glDrawElements(type, count, GL_UNSIGNED_INT, 0);
	}
}