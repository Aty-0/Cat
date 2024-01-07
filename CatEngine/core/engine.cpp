#include "engine.h"

#include "core/input_manager.h"
#include "core/game_window.h"
#include "core/utils/logger.h"
#include "core/utils/game_time.h"
#include "core/callback_storage.h"
#include "physics/physics_core.h"
#include "graphics/renderer.h"
#include "io/resource_manager.h"

#include "game/scene/scene_manager.h"
#include "scripts/script_core.h"
#include "core/editor/filesystem_tree_ui.h"

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

		VERB("Initialize game window...");
		m_window = core::game_window::get_instance();
		if (!m_window->create())
			return false;

		VERB("Initialize renderer...");
		m_renderer = graphics::renderer::get_instance();		
		if (!m_renderer->init())
			return false;


		VERB("Initialize input manager...");
		m_input = core::input_manager::get_instance();
		m_input->init();

		m_time = core::utils::game_time::get_instance();

		VERB("Initialize resource manager...");
		m_rm = io::resource_manager::get_instance();
		
		// TODO: Remove it 
		VERB("Allocate global update callback");
		m_on_global_update = new core::callback_storage();

		VERB("Initialize physics core...");
		const static auto pc = physics::physics_core::get_instance();
		pc->init();

		VERB("Initialize post process in render...");
		m_renderer->init_post_process();
		
		VERB("Run scene manager...");
		m_sm = game::scene::scene_manager::get_instance();
		m_sm->create();

		
		VERB("Run main script and initialize script core...");
		const static auto sc = scripts::script_core::get_instance();
		CAT_ASSERT(sc->run("main"));			
		sc->run_func("main", "cat_main");

		VERB("Add onImGuiRender callbacks editor windows...");
		m_renderer->onImGuiRender.add(std::bind(&core::utils::logger::render_console, core::utils::logger::get_instance()));
		m_renderer->onImGuiRender.add(std::bind(&core::editor::filesystem_tree_ui::render, core::editor::filesystem_tree_ui::get_instance()));
		
		VERB("Add engine input callbacks...");
		// add exit hotkey
		m_input->add_listener(input_key_code::KEYBOARD_ESCAPE, input_key_state::Press, input_device::Keyboard,
			std::bind(&core::engine::destroy, this));
		m_input->add_listener(input_key_code::KEYBOARD_F8, input_key_state::Press, input_device::Keyboard,
			std::bind(&graphics::renderer::toggle_imgui_rendering, m_renderer));


		// if window is active we are call onLoop function
		INFO("Run loop...");
		while (!m_window->is_close())
		{
			on_loop();
		}

		// if loop are breaked we are destroy engine
		destroy();

		return true;
	}

	void engine::on_update(float DeltaTime)
	{
		m_sm->update(DeltaTime);
		m_input->update();

		m_on_global_update->run_all();
	}

	bool engine::init_GLFW()
	{
		INFO("Initialize GLFW...");

		if (!glfwInit())
		{			
			ERR("Failed to init glfw!");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		return true;
	}

	void engine::on_loop()
	{
		m_window->pool();
		// Update things
		m_time->tick();
		on_update(m_time->get_delta_time());
		// Renderer update
		m_renderer->render();
		m_window->swap();
	}


	callback_storage* engine::get_on_global_update() const
	{
		return m_on_global_update;
	}

	void engine::destroy()
	{
		INFO("Engine destroy");
		
		m_on_global_update->clear();

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