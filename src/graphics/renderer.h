#pragma once
#include "core/common.h"

#include "Libs/imgui/imgui.h"

namespace cat::core
{
	class game_window;
	class callback_storage;
	namespace utils
	{
		class game_time;
	}
}

namespace cat::graphics
{
	class frame_buffer;
	class piece;

	class CATENGINE_API renderer : public core::utils::singleton<renderer>
	{
	public:
		renderer();
		~renderer();

		bool init();
		void render();
		void destroy();

		void setPolygonMode(std::int32_t mode);
		void cull(std::int32_t cullmode = GL_BACK, std::int32_t frontface = GL_CW);
		void disableCull();

		void draw(std::int32_t count, std::int32_t type);
		void drawElements(std::int32_t count, std::int32_t type);

		void initPostProcess();
		void imguiToggleVisibility();


		void debugDrawLine(glm::vec3 begin, glm::vec3 end, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		void debugDrawCube(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	private:
		bool imguiInit();
		void imguiRenderDebugWindow();
		void imguiRender();
		void imguiNewFrame();
		void drawPostProcessPiece();
		void recreateFrameBuffer();
		void debugPrintExistingExt();
		
		static void onGetError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);



		std::shared_ptr<frame_buffer> m_postProcessFramebuffer;
		graphics::piece* m_postProcessPiece;
		core::game_window* m_window;
		core::utils::game_time* m_time;

	
		bool m_disable_post_proc;
		bool m_renderImgui;
	public:
		static core::callback_storage onImGuiRender;


	};
}

