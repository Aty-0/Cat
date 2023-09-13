﻿#include "engine.h"

#include "core/input_manager.h"
#include "core/utils/game_time.h"
#include "core/game_window.h"
#include "graphics/renderer.h"
#include "core/utils/logger.h"
#include "io/resource_manager.h"
#include "game/scene/scene_manager.h"

#include "scripts/scripts_core.h"
// TODO: Remove
#include "game/game_object.h"
#include "game/components/drawable.h"
#include "game/components/test_comp_rotator.h"


namespace cat::core
{
	engine::engine()
	{

	}

	engine::~engine()
	{

	}

	bool engine::run() 
	{		
		INFO("Initialize engine...");

		if (!init_GLFW())
			return false;

		m_window = core::game_window::get_instance();
		if (!m_window->create())
			return false;

		m_renderer = graphics::renderer::get_instance();		
		if (!m_renderer->init())
			return false;


		m_input = core::input_manager::get_instance();
		m_input->init();

		// add exit hotkey
		m_input->add_listener(input_key_code::KEYBOARD_ESCAPE, input_key_state::Press, input_device::Keyboard, 
			std::bind(&core::engine::destroy, this));

		m_time = core::utils::game_time::get_instance();
		INFO("Run loop...");


		m_rm = io::resource_manager::get_instance();

//#define TEST_RESOURCES		

#ifdef TEST_RESOURCES		
		auto r_exist = m_rm->is_exist<graphics::shader>("test_f", {"glsl"});
		INFO("r_exist %s", r_exist == true ? "true" : "false");

		auto r_size = m_rm->size<graphics::shader>("test_f", {"glsl"});
		INFO("size in bytes %i", r_size);

		auto r_data = m_rm->read<const char*, graphics::shader>("test_f", {"glsl"});
		INFO("data \n\n %s", r_data);
#endif

		m_sm = game::scene::scene_manager::get_instance();
		m_sm->create();
		auto hui = m_sm->get_game_object_name("hui");

		auto test_object = m_sm->create_game_object<game::game_object>("SomeTests", CAT_DEFAULT_GAMEOBJECT_TYPE, CAT_DEFAULT_GAMEOBJECT_PREFIX);
		test_object->create_component<game::components::drawable>();
		test_object->create_component<game::components::test_comp_rotator>();
		 for (std::int32_t i = 1; i <= 2; i++)
		 {
		 	auto obj = m_sm->create_game_object<game::game_object>("SomeTests" + std::to_string(i),
		 		CAT_DEFAULT_GAMEOBJECT_TYPE, CAT_DEFAULT_GAMEOBJECT_PREFIX,
		 		glm::vec3(i * 2, 0, 0), VEC3_ZERO, glm::vec3(0.4f, 0.4f, 0.4f));
		 	auto dr = obj->create_component<game::components::drawable>();
		 }
		 

		m_input->add_listener(input_key_code::KEYBOARD_0, input_key_state::Press, input_device::Keyboard,
			std::bind(&core::engine::run_test_script, this));

		// if window is active we are call onLoop function
		while (!m_window->is_close())
		{
			on_loop();
		}

		// if loop are breaked we are destroy engine
		destroy();

		return true;
	}

	void engine::run_test_script()
	{
		auto scripts = scripts::script_core::get_instance();
		scripts->run("test");
	}

	void engine::on_update(float DeltaTime)
	{
		m_sm->update(DeltaTime);
		m_input->update();
	}

	bool engine::init_GLFW()
	{
		INFO("Initialize GLFW...");

		if (!glfwInit())
		{			
			ERR("Failed to init glfw!");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		return true;
	}

	void engine::on_loop()
	{
		glfwPollEvents();
		// Update things
		m_time->tick();
		on_update(m_time->get_delta_time());
		// Renderer update
		m_renderer->render();

		glfwSwapBuffers(m_window->get_GLFW_window());
	}


	void engine::destroy()
	{
		INFO("Engine destroy");
		
		m_window->destroy();
		m_renderer->destroy();

		
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}

	bool engine::load_all()
	{
		INFO("Load all...");
		return true;
	}
}