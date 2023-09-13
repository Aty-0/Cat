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

		if (!init_GLAD())
			return false;
		
		if (!init_imgui())
			return false;

		m_time = core::utils::game_time::get_instance();

		return true;
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

	bool renderer::init_GLAD()
	{
		INFO("Initialize glad...");

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			FATAL("Failed to init glad!");
			return false;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return true;
	}

	void renderer::draw_elements(std::int32_t count, std::int32_t type)
	{
		CAT_GL_SAFE_CHK(glDrawElements(type, count, GL_UNSIGNED_INT, 0));
	}
}