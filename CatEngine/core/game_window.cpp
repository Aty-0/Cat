#include "game_window.h"
#include "core/utils/logger.h"
#include "core/callback_storage.h"

#include "io/resource_manager.h"

namespace cat::core
{
	callback_storage game_window::onWindowResize;
	callback_storage game_window::onWindowResized;

	game_window::game_window() :
		m_window(nullptr)
	{

	}

	game_window::~game_window()
	{

	}

	bool game_window::create(std::uint32_t w, std::uint32_t h, const char* title)
	{		
		INFO("Create game window...");

		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
		if ((m_window = glfwCreateWindow(w, h, title, NULL, NULL)) == nullptr)
		{
			FATAL("Failed to create GLFW window!");
			return false;
		}
		glfwMakeContextCurrent(m_window);
		glfwSetErrorCallback(on_get_error);
		glfwSetDropCallback(m_window, on_drop_callback);
		// TODO or FIXME: That's two callback behave the same, so what's i need to use
		//				  Need to check GLFW documentation
		glfwSetWindowSizeCallback(m_window, on_window_size_change);
		glfwSetFramebufferSizeCallback(m_window, on_framebuffer_size_change);
		
		set_vsync(0);

		return true;
	}

	void game_window::on_drop_callback(GLFWwindow* window, std::int32_t count, const char** paths)
	{
		static const auto rm = io::resource_manager::get_instance();
		rm->move_files_to_data(count, paths);		
	}

	void game_window::set_vsync(bool vsync)
	{
		m_vsync = vsync;
		glfwSwapInterval(static_cast<std::int32_t>(m_vsync));
	}

	void game_window::on_get_error(std::int32_t error_code, const char* description)
	{
		ERR("GLFW Error %d %s", error_code, description);
	}

	// FIXME ? Looks wierd a bit 
	static bool isFullyResized = false;

	void game_window::on_window_size_change(GLFWwindow* window, std::int32_t width, std::int32_t height)
	{
		static const auto gw = get_instance();
		gw->m_resized = false;
		isFullyResized = false;
		
		gw->onWindowResize();
	}

	void game_window::swap()
	{
		if (m_resized && !isFullyResized)
		{
			isFullyResized = true;
			onWindowResized();
		}

		glfwSwapBuffers(m_window);
	}

	void game_window::pool()
	{
		m_resized = true;
		glfwPollEvents();
	}

	void game_window::on_framebuffer_size_change(GLFWwindow* window, std::int32_t width, std::int32_t height)
	{	
		glViewport(0, 0, width, height);
	}

	void game_window::set_top(std::int32_t top)
	{
		glfwSetWindowPos(m_window, get_left(), top);
	}

	void game_window::set_left(std::int32_t left)
	{
		glfwSetWindowPos(m_window, left, get_top());
	}

	void game_window::set_width(std::uint32_t w)
	{
		glfwSetWindowSize(m_window, w, get_height());
	}

	void game_window::set_height(std::uint32_t h)
	{
		glfwSetWindowSize(m_window, get_width(), h);
	}

	void game_window::destroy()
	{
		glfwDestroyWindow(m_window);
	}

	std::int32_t game_window::get_width()		const
	{
		std::int32_t w = 0; 
		glfwGetWindowSize(m_window, &w, 0);
		return w;
	}

	std::int32_t game_window::get_height()		const
	{ 
		std::int32_t h = 0;
		glfwGetWindowSize(m_window, 0, &h);
		return h;
	}

	std::int32_t game_window::get_top()		const
	{ 
		std::int32_t t = 0;
		glfwGetWindowPos(m_window, 0, &t);
		return t; 
	}

	std::int32_t game_window::get_left()		const
	{ 
		std::int32_t l = 0;
		glfwGetWindowPos(m_window, &l, 0);
		return l;
	}

	bool game_window::is_resized()		const
	{
		return m_resized;
	}

	bool game_window::get_vsync()		const
	{
		return m_vsync;
	}

	bool game_window::is_close()		const
	{
		return static_cast<bool>(glfwWindowShouldClose(m_window)); 
	}

	GLFWwindow* game_window::get_GLFW_window() const
	{ 
		return m_window; 
	}

}