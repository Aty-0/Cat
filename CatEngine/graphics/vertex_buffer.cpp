#include "graphics/vertex_buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	vertex_buffer::vertex_buffer()
	{

	}

	vertex_buffer::~vertex_buffer() 
	{

	}

	void vertex_buffer::bind()
	{
		CAT_GL_SAFE_CHK(glBindVertexArray(m_vao));
	}

	void vertex_buffer::gen()
	{
		 CAT_GL_SAFE_CHK(glGenVertexArrays(1, &m_vao));
		 CAT_GL_SAFE_CHK(glBindVertexArray(m_vao));
		 CAT_GL_SAFE_CHK(glGenBuffers(1, &m_vbo));
	}

	void vertex_buffer::clear()
	{
		CAT_GL_SAFE_CHK(glDeleteBuffers(1, &m_vao));
		CAT_GL_SAFE_CHK(glDeleteBuffers(1, &m_vbo));

		m_data = nullptr; // reset data pointer
	}

	void vertex_buffer::set_attrib(std::uint32_t num, std::uint32_t size,
		GLenum type, std::int32_t typesize, const void* offset)
	{
		glEnableVertexAttribArray(num);
		glVertexAttribPointer(num, size, type, GL_FALSE, typesize, offset);
	}
}