#include "shader.h"
#include "core/utils/logger.h"
#include "io/resource_manager.h"
#include "core/utils/game_time.h"

#include "game/game_object.h"
#include "game/components/camera.h"
#include "game/scene/scene_manager.h"
#include "core/game_window.h"

namespace cat::graphics
{
	shader::shader() :
		m_vertex_shader(0),
		m_fragment_shader(0),
		m_shader_program(0)
	{

	}

	shader::~shader()
	{		
		glDeleteProgram(m_shader_program);
	}

	bool shader::load(const char* name)
	{
		static const auto rm = io::resource_manager::getInstance();
		auto vertex_data = rm->get<const char*, shader>((std::string(name) + "_v").c_str(), {"glsl"});
		if (!compile(vertex_data, GL_VERTEX_SHADER, m_vertex_shader))
		{
			return false;
		}	
		
		auto frag_data = rm->get<const char*, shader>((std::string(name) + "_f").c_str(), {"glsl"});
		if (!compile(frag_data, GL_FRAGMENT_SHADER, m_fragment_shader))
		{
			return false;
		}
		
		VERB("shader::load shader program stage... ");
		std::int32_t status = 0;
		m_shader_program = glCreateProgram();
		glAttachShader(m_shader_program, m_vertex_shader);
		glAttachShader(m_shader_program, m_fragment_shader);
		glLinkProgram(m_shader_program);

		// check for linking errors
		glGetProgramiv(m_shader_program, GL_LINK_STATUS, &status);

		if (!status)
		{
			char err_buffer[512] = "\0";
	
			glGetShaderInfoLog(m_shader_program, sizeof(err_buffer), NULL, err_buffer);

			ERR("link error");

			if (err_buffer == NULL || !err_buffer && err_buffer[0] == '\0')
				ERR("%s", err_buffer);
			return false;
		}

		// Cleanup shaders
		glDeleteShader(m_vertex_shader);
		glDeleteShader(m_fragment_shader);

		return true;
	}

	const char* shader::getShaderType(std::int32_t type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER:
			return "Vertex";
		case GL_FRAGMENT_SHADER:
			return "Fragment";
		case GL_GEOMETRY_SHADER:
			return "Geometry";
		default:
			return "Unknown";
		}
		return "";
	}


	bool shader::compile(const char* data, std::int32_t type, std::uint32_t& shader)
	{
		const auto type_str = getShaderType(type);
		VERB("shader::compile %s", type_str);

		shader = glCreateShader(type);
		glShaderSource(shader, 1, &data, NULL);
		glCompileShader(shader);
		
		std::int32_t status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			char err_buffer[512] = "\0";
			
			glGetShaderInfoLog(shader, sizeof(err_buffer), NULL, err_buffer);
			if (err_buffer == NULL || !err_buffer && err_buffer[0] == '\0')
			{
				ERR("%s shader compilation failed \nNo details", type_str);
			}
			else
			{
				ERR("%s shader compilation failed \n%s", type_str, err_buffer);
			}
			return false;
		}
		
	
		return true;
	}

	void shader::updateDefaultUniforms()
	{
		static const auto time = core::utils::game_time::getInstance();

		
		setFloat("base.delta_time", time->getDeltaTime());
		setFloat("base.time", time->getTime());

		// TODO: Move to constructor 
		//		 Make static 
		static const auto sm = game::scene::scene_manager::getInstance();
		static const auto camera_go = sm->getGameObjectName<>(game::components::camera::EngineCameraName);
		
		// FIXME: Make it simpler 
		static const auto window = core::game_window::getInstance();
		static auto aspect_ratio = static_cast<float>(window->getWidth() / window->getHeight());
		setFloat("base.aspect_ratio", aspect_ratio);

	
		if (camera_go != nullptr)
		{
			static const auto camera_comp = camera_go->getComponent<game::components::camera>();

			if (camera_comp != nullptr)
			{
				setMat4("transform.view", camera_comp->getView());
				setMat4("transform.projection", camera_comp->getProjection());
			}
		}

	}

	std::uint32_t shader::getProgram() const
	{
		return m_shader_program;
	}

	void shader::bind()
	{
		if (m_shader_program != 0)
		{
			glUseProgram(m_shader_program);
			updateDefaultUniforms();
		}
	}
	
	void shader::unbind()
	{
		glUseProgram(0);	
	}

	void shader::setInt32(const char* name, std::int32_t value)
	{
		glUniform1i(glGetUniformLocation(m_shader_program, name), value);
	}

	void shader::setUInt32(const char* name, std::uint32_t value)
	{
		glUniform1i(glGetUniformLocation(m_shader_program, name), value);
	}

	void shader::setFloat(const char* name, float value)
	{
		glUniform1f(glGetUniformLocation(m_shader_program, name), value);
	}

	void shader::setBool(const char* name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_shader_program, name), static_cast<std::int32_t>(value));
	}

	void shader::setVec2(const char* name, glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(m_shader_program, name), 1, &value[0]);
	}

	void shader::setVec3(const char* name, glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(m_shader_program, name), 1, &value[0]);
	}

	void shader::setVec4(const char* name, glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(m_shader_program, name), 1, &value[0]);
	}

	void shader::setMat2(const char* name, glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, &mat[0][0]);
	}

	void shader::setMat3(const char* name, glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, &mat[0][0]);
	}

	void shader::setMat4(const char* name, glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, &mat[0][0]);
	}
}