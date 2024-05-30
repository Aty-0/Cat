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

		bool create(std::uint32_t w = 1280, std::uint32_t h = 768, const char* title = "Cat");
		
		void setVsync(bool vsync);
		void setTop(std::int32_t top);
		void setLeft(std::int32_t left);
		void setWidth(std::uint32_t w);
		void setHeight(std::uint32_t h);
			 
		void destroy();
		void swap();
		void pool();

		[[nodiscard]] inline std::int32_t getWidth()		const;
		[[nodiscard]] inline std::int32_t getHeight()		const;
		[[nodiscard]] inline std::int32_t getTop()		const;
		[[nodiscard]] inline std::int32_t getLeft()		const;
		[[nodiscard]] inline bool isClose()		const;
		[[nodiscard]] inline bool getVsync()		const;
		[[nodiscard]] inline bool isResized()		const;
		[[nodiscard]] inline GLFWwindow* getGLFWWindowInstance() const;

	private:
		GLFWwindow* m_window;
		bool	m_resized;
		bool	m_vsync;
		// Callbacks
		static void				onWindowSizeChange(GLFWwindow* window, std::int32_t width, std::int32_t height);
		static void				onFramebufferSizeChange(GLFWwindow* window, std::int32_t width, std::int32_t height);
		static void				onGetGLFWError(std::int32_t error_code, const char* description);
		static void				onDropCallback(GLFWwindow* window, std::int32_t count, const char** paths);
	public:
		static callback_storage onWindowResize;
		static callback_storage onWindowResized;
	};
}