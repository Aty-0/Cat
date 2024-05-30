#pragma once
#include "core/common.h"

namespace cat::graphics
{
	class CATENGINE_API shader
	{
	public:
		shader();
		~shader();

		bool load(const char* name);

		// TODO: get rid of this funcs, need make something better 
		void setInt32(const char* name, std::int32_t value);
		void setUInt32(const char* name, std::uint32_t value);
		void setFloat(const char* name, float value);
		void setBool(const char* name, bool value);
		void setVec2(const char* name, glm::vec2& value);
		void setVec3(const char* name, glm::vec3& value);
		void setVec4(const char* name, glm::vec4& value);
		void setMat2(const char* name, glm::mat2& mat);
		void setMat3(const char* name, glm::mat3& mat);
		void setMat4(const char* name, glm::mat4& mat);


		void bind();
		void unbind();
		[[nodiscard]] inline std::uint32_t getProgram() const;

	private:
		bool compile(const char* data, std::int32_t type, std::uint32_t& shader);
		void updateDefaultUniforms();
		inline const char* getShaderType(std::int32_t type);

		std::uint32_t			m_shader_program;
		std::uint32_t			m_vertex_shader;
		std::uint32_t			m_fragment_shader;

	};
}