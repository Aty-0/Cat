#pragma once
#include "core/common.h"

namespace cat::graphics
{
	class shader
	{
	public:
		shader();
		~shader();

		bool					load(const char* name);


		// TODO: get rid of this funcs, need make something better 
		void set_int32(const char* name, std::int32_t value);
		void set_uint32(const char* name, std::uint32_t value);
		void set_float(const char* name, float value);
		void set_bool(const char* name, bool value);
		void set_vec2(const char* name, glm::vec2& value);
		void set_vec3(const char* name, glm::vec3& value);
		void set_vec4(const char* name, glm::vec4& value);
		void set_mat2(const char* name, glm::mat2& mat);
		void set_mat3(const char* name, glm::mat3& mat);
		void set_mat4(const char* name, glm::mat4& mat);


		void bind();
		inline std::uint32_t get_program() const;

	private:
		bool compile(const char* data, std::int32_t type, std::uint32_t& shader);
		void update_default_uniforms();
		const char* get_shader_type(std::int32_t type);

		std::uint32_t			m_shader_program;
		std::uint32_t			m_vertex_shader;
		std::uint32_t			m_fragment_shader;

	};
}