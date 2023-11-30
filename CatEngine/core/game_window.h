#pragma once
#include "core/common.h"

namespace cat::core
{
	class callback_storage;
	class CATENGINE_API game_window : public core::utils::singleton<game_window>
	{
	public:
		game_window();
		~game_window();

		bool create(std::uint32_t w = 1280, std::uint32_t h = 768, const char* title = "Engine");
		
		void set_vsync(bool vsync);
		void set_top(std::int32_t top);
		void set_left(std::int32_t left);
		void set_width(std::uint32_t w);
		void set_height(std::uint32_t h);
			 
		void destroy();
		void swap();
		void pool();

		[[nodiscard]] inline std::int32_t get_width()		const;
		[[nodiscard]] inline std::int32_t get_height()		const;
		[[nodiscard]] inline std::int32_t get_top()		const;
		[[nodiscard]] inline std::int32_t get_left()		const;
		[[nodiscard]] inline bool is_close()		const;
		[[nodiscard]] inline bool get_vsync()		const;
		[[nodiscard]] inline bool is_resized()		const;
		[[nodiscard]] inline GLFWwindow* get_GLFW_window() const;

	private:
		GLFWwindow* m_window;
		bool	m_resized;
		bool	m_vsync;
		// Callbacks
		static void				on_window_size_change(GLFWwindow* window, std::int32_t width, std::int32_t height);
		static void				on_framebuffer_size_change(GLFWwindow* window, std::int32_t width, std::int32_t height);
		static void				on_get_error(std::int32_t error_code, const char* description);
		static void				on_drop_callback(GLFWwindow* window, std::int32_t count, const char** paths);
	public:
		static callback_storage onWindowResize;
		static callback_storage onWindowResized;
	};
}