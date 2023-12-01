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
		if (m_vao)
		{
			glBindVertexArray(m_vao);
		}
	}

	void vertex_buffer::gen()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);
	}

	void vertex_buffer::clear()
	{
		if (m_vao)
		{
			glDeleteBuffers(1, &m_vao);

		}
		
		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
		}

		m_data = nullptr; // reset data pointer
	}

	void vertex_buffer::set_attrib(std::uint32_t num, std::uint32_t size,
		GLenum type, std::int32_t typesize, const void* offset)
	{
		glEnableVertexAttribArray(num);
		glVertexAttribPointer(num, size, type, GL_FALSE, typesize, offset);
	}

	void vertex_buffer::unbind_buffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void vertex_buffer::unbind_buffer_array()
	{
		glBindVertexArray(0);
	}
}