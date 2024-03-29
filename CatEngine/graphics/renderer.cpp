#include "renderer.h"

#include "core/callback_storage.h"
#include "core/utils/logger.h"
#include "core/game_window.h"
#include "core/utils/game_time.h"

#include "game/scene/scene_manager.h"
#include "game/components/camera.h"

#include "Libs/imgui/imgui_impl_opengl3.h"
#include "Libs/imgui/imgui_impl_glfw.h"

#include "graphics/frame_buffer.h"
#include "graphics/piece.h"

namespace cat::graphics
{
	core::callback_storage renderer::onImGuiRender;

	renderer::renderer() : 
		m_renderImgui(true), 
		m_postProcessFramebuffer(nullptr),
		m_postProcessPiece(nullptr),
		m_disable_post_proc(false),
		m_time(nullptr),
		m_window(nullptr)
	{

	}

	renderer::~renderer()
	{

	}

	void renderer::imguiToggleVisibility()
	{
		m_renderImgui =! m_renderImgui;
	}

	void renderer::destroy()
	{
		INFO("Renderer destroy...");

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		core::utils::safe_delete(m_postProcessPiece);
		// FIXME:
		//m_postProcessFramebuffer.reset();
		m_time = nullptr;
		m_window = nullptr;
	}

	void renderer::debugPrintExistingExt()
	{
		std::int32_t ext_num = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &ext_num);
		
		for (std::int32_t i = 0; i < ext_num; i++)
		{
			const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			INFO("%i Extension %s", i, extension);
		}
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

		//debug_print_existing_ext();

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(onGetError, nullptr);
		
		if (!imguiInit())
			return false;
		
		m_time = core::utils::game_time::get_instance();

		onImGuiRender.add(std::bind(&renderer::imguiRenderDebugWindow, this));
		return true;
	}

	void renderer::setPolygonMode(std::int32_t mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

	void renderer::cull(std::int32_t cullmode, std::int32_t frontface)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(cullmode);
		glFrontFace(frontface);
	}

	void renderer::disableCull()
	{
		glDisable(GL_CULL_FACE);
	}

	void renderer::onGetError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		const auto source_str = [source]() {
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

		const auto type_str = [type]() {
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

		const auto severity_str = [severity]() {
			switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "UNKNOWN";
			}
		}();

		static const bool verbose = false;
		
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
			if(verbose)
				VERB("GL: %s %s %s %s", source_str, type_str, severity_str, message);
			break;
		}

	}

	void renderer::render()
	{
		if (m_window->get_width() == 0 
			|| m_window->get_height() == 0)
			return;

		imguiNewFrame();

		if (m_disable_post_proc)
		{
			m_postProcessFramebuffer->bind();
		}

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static const auto sm = game::scene::scene_manager::get_instance();
		sm->render(this);

		glDisable(GL_DEPTH_TEST);
		// TODO: UI rendering will be located there 
		
		if (m_disable_post_proc)
		{
			m_postProcessFramebuffer->unbind_buffer();
			
			// Clear all relevant buffers
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			drawPostProcessPiece();
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool renderer::imguiInit()
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
	
	void renderer::imguiRenderDebugWindow()
	{
		ImGui::Begin("Debug window");

		ImGui::Text("FPS %d", m_time->get_fps());
		ImGui::Text("Time %f", m_time->get_time());
		ImGui::Text("Delta time %f", m_time->get_delta_time());
		if (ImGui::Checkbox("Disable Post Process", &m_disable_post_proc))
		{
			recreateFrameBuffer();
		}

		ImGui::End();
	}

	void renderer::imguiRender()
	{
		onImGuiRender.run_all();		
	}

	void renderer::imguiNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (m_renderImgui)
		{
			imguiRender();
		}

		ImGui::Render();
	}

	void renderer::draw(std::int32_t count, std::int32_t type)
	{
		glDrawArrays(type, 0, count);
	}

	void renderer::drawElements(std::int32_t count, std::int32_t type)
	{
		glDrawElements(type, count, GL_UNSIGNED_INT, 0);
	}

	void renderer::initPostProcess()
	{
		m_postProcessFramebuffer = std::make_shared<frame_buffer>(*new frame_buffer());
		m_postProcessFramebuffer->gen();
		std::vector<graphics::vertex> vertices = { { glm::vec3(1.0f,  1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
											{ glm::vec3(1.0f, -1.0f, 0.0f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
											{ glm::vec3(-1.0f, -1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 0.0f) },
											{ glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 1.0f) }
		};
		
		std::vector<std::uint32_t> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		m_postProcessPiece = new graphics::piece(vertices, indices, {}, "postprocess");
		m_postProcessPiece->setTexture(0, m_postProcessFramebuffer->getTextureShared());

		core::game_window::onWindowResized.add(std::bind(&graphics::renderer::recreateFrameBuffer, this));
	}

	void renderer::recreateFrameBuffer()
	{
		if (m_window->get_width() == 0
			|| m_window->get_height() == 0 
			|| !m_disable_post_proc
			|| !m_window->is_resized())
			return;
		
		// Remove old and create new one 
		m_postProcessFramebuffer->clear();
		m_postProcessFramebuffer->gen();
	}
	
	void renderer::drawPostProcessPiece()
	{
		m_postProcessPiece->begin();
		m_postProcessPiece->end(this);
	}

	void renderer::debugDrawLine(glm::vec3 begin, glm::vec3 end)
	{
		const std::vector<graphics::vertex> vertices = 
		{ 
			{ begin, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
			{ end,   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		};

		// FIX ME: Memory leak...
		static graphics::piece* piece = nullptr; 
		
		if (piece == nullptr)
		{
			piece = new graphics::piece(vertices, { }, { }, "debug");
			piece->setPolyMode(GL_LINE);
		}
		
		
		piece->begin();
		
		const auto shader = piece->getShader();
		glm::mat4 world = glm::mat4();
		world = glm::translate(world, end) * glm::scale(world, glm::vec3(1,1,1));
		
		shader->set_mat4("transform.world", world);
		
		piece->end(this);

		core::utils::safe_delete(piece);
	}

}