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
	callback_storage* engine::onGlobalUpdate = nullptr;

	engine::engine()
	{

	}

	engine::~engine()
	{

	}

	bool engine::run() 
	{		
		INFO("Initialize engine...");

		if (!initGLFW())
			return false;

		VERB("Initialize game window...");
		m_window = core::game_window::getInstance();
		if (!m_window->create())
			return false;

		VERB("Initialize renderer...");
		m_renderer = graphics::renderer::getInstance();
		if (!m_renderer->init())
			return false;

		VERB("Initialize input manager...");
		m_input = core::input_manager::getInstance();
		m_input->init();

		m_time = core::utils::game_time::getInstance();

		VERB("Initialize resource manager...");
		m_rm = io::resource_manager::getInstance();
		

		VERB("Initialize physics core...");
		m_physics = physics::physics_core::getInstance();
		m_physics->init();

		VERB("Initialize post process in renderer...");
		m_renderer->initPostProcess();
		
		VERB("Run scene manager...");
		m_sm = game::scene::scene_manager::getInstance();
		m_sm->create();

		
		VERB("Run main script and initialize script core...");
		onGlobalUpdate = new core::callback_storage();
		const static auto sc = scripts::script_core::getInstance();
		CAT_ASSERT(sc->run("main"));			
		sc->runFunc("main", "cat_main");

		VERB("Add some editor windows...");
		m_renderer->onImGuiRender.add(std::bind(&core::utils::logger::draw, core::utils::logger::getInstance()));
		m_renderer->onImGuiRender.add(std::bind(&core::editor::filesystem_tree_ui::render, core::editor::filesystem_tree_ui::getInstance()));
		
		VERB("Add engine input callbacks...");
		// add exit hotkey
		m_input->addListener(input_key_code::KEYBOARD_ESCAPE, input_key_state::Press, input_device::Keyboard,
			std::bind(&core::engine::destroy, this));
		m_input->addListener(input_key_code::KEYBOARD_F8, input_key_state::Press, input_device::Keyboard,
			std::bind(&graphics::renderer::imguiToggleVisibility, m_renderer));


		// if window is active we are call onLoop function
		INFO("Run loop...");
		while (!m_window->isClose())
		{
			onLoop();
		}

		// if loop are breaked we are destroy engine
		destroy();

		return true;
	}

	void engine::onUpdate(float DeltaTime)
	{
		m_physics->update(DeltaTime);
		m_sm->update(DeltaTime);
		m_input->update();
		onGlobalUpdate->runAll();
	}

	bool engine::initGLFW()
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

	void engine::onLoop()
	{
		m_window->pool();
		// Update things
		m_time->tick();
		onUpdate(m_time->getDeltaTime());
		// Renderer update
		m_renderer->render();
		m_window->swap();
	}

	void engine::destroy()
	{
		INFO("Engine destroy");
		onGlobalUpdate->clear();
		m_physics->destroy();
		m_sm->clear();
		m_window->destroy();
		m_renderer->destroy();
		
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}