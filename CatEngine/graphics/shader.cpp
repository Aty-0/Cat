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
		glDeleteShader(m_shader_program);
	}

	bool shader::load(const char* name)
	{
		static const auto rm = io::resource_manager::get_instance();
		auto vertex_data = rm->read<const char*, shader>((std::string(name) + "_v").c_str(), {"glsl"});
		if (!compile(vertex_data, GL_VERTEX_SHADER, m_vertex_shader))
		{
			return false;
		}

		auto frag_data = rm->read<const char*, shader>((std::string(name) + "_f").c_str(), {"glsl"});
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

		char err_buffer[512];
		if (!status)
		{
			glGetShaderInfoLog(m_shader_program, sizeof(err_buffer), NULL, err_buffer);
			ERR("link error\n%s", err_buffer);
			return false;
		}

		glDeleteShader(m_vertex_shader);
		glDeleteShader(m_fragment_shader);
		return true;
	}

	const char* shader::get_shader_type(std::int32_t type)
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
		const auto type_str = get_shader_type(type);
		VERB("shader::compile %s", type_str);

		shader = glCreateShader(type);
		glShaderSource(shader, 1, &data, NULL);
		glCompileShader(shader);
		
		std::int32_t status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		char err_buffer[512];
		if (!status)
		{
			glGetShaderInfoLog(shader, sizeof(err_buffer), NULL, err_buffer);
			ERR("%s shader compilation failed \n%s", type_str, err_buffer);
			return false;
		}

	
		return true;
	}


	void shader::update_default_uniforms()
	{
		static const auto time = core::utils::game_time::get_instance();

		set_float("delta_time", time->get_delta_time());
		set_float("time",		time->get_time());

		// TODO: Move to constructor 
		//		 Make static 
		static const auto sm = game::scene::scene_manager::get_instance();
		static const auto camera_go = sm->get_game_object_name<>(game::components::camera::EngineCameraName);
		
		// FIXME: Make it simpler 
		static const auto window = core::game_window::get_instance();

		auto aspect_ratio = window->get_width() / window->get_height();
		set_float("aspect_ratio", aspect_ratio);

		if (camera_go != nullptr)
		{
			static const auto camera_comp = camera_go->get_component<game::components::camera>();

			if (camera_comp != nullptr)
			{
				set_mat4("view", camera_comp->get_view());
				set_mat4("projection", camera_comp->get_projection());
			}
		}

	}

	void shader::bind()
	{
		glUseProgram(m_shader_program);
		update_default_uniforms();
	}

	void shader::set_int32(const char* name, std::int32_t value)
	{
		glUniform1i(glGetUniformLocation(m_shader_program, name), value);
	}

	void shader::set_uint32(const char* name, std::uint32_t value)
	{
		glUniform1i(glGetUniformLocation(m_shader_program, name), value);
	}

	void shader::set_float(const char* name, float value)
	{
		glUniform1f(glGetUniformLocation(m_shader_program, name), value);
	}

	void shader::set_bool(const char* name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_shader_program, name), (std::int32_t)value);
	}

	void shader::set_vec2(const char* name, glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(m_shader_program, name), 1, &value[0]);
	}

	void shader::set_vec3(const char* name, glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(m_shader_program, name), 1, &value[0]);
	}

	void shader::set_vec4(const char* name, glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(m_shader_program, name), 1, &value[0]);
	}

	void shader::set_mat2(const char* name, glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, &mat[0][0]);
	}

	void shader::set_mat3(const char* name, glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, &mat[0][0]);
	}

	void shader::set_mat4(const char* name, glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, &mat[0][0]);
	}
}