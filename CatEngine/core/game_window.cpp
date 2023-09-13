#include "game_window.h"
#include "core/utils/logger.h"

namespace cat::core
{
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
		glfwSetFramebufferSizeCallback(m_window, on_framebuffer_size_change);
		glfwSetErrorCallback(on_get_error);
		glfwSwapInterval(0);

		return true;
	}

	void game_window::on_get_error(std::int32_t error_code, const char* description)
	{
		ERR("GLFW Error %d %s", error_code, description);
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

	bool game_window::is_close()		const
	{
		return static_cast<bool>(glfwWindowShouldClose(m_window)); 
	}

	GLFWwindow* game_window::get_GLFW_window() const
	{ 
		return m_window; 
	}

}