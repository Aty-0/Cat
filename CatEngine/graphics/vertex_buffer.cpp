#include "graphics/vertex_buffer.h"
#include "core/utils/logger.h"
#include "graphics/vertex.h"

namespace cat::graphics
{
	vertex_buffer::vertex_buffer()
	{

	}

	vertex_buffer::~vertex_buffer() 
	{
		clear();
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
			glDeleteVertexArrays(1, &m_vao);
		}
		
		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
		}

		m_data.clear();
		m_data.shrink_to_fit();
	}

	void vertex_buffer::setAttrib(std::uint32_t num, std::uint32_t size,
		GLenum type, std::int32_t typesize, const void* offset)
	{
		glEnableVertexAttribArray(num);
		glVertexAttribPointer(num, size, type, GL_FALSE, typesize, offset);
	}

	void vertex_buffer::unbindBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void vertex_buffer::unbindBufferArray()
	{
		glBindVertexArray(0);
	}

	void vertex_buffer::setBufferData(const std::vector<graphics::vertex>& data, std::uint32_t draw)
	{
		m_data = data;
		CAT_ASSERT(!m_data.empty());
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, size() * sizeof(graphics::vertex), &data[0], GL_STATIC_DRAW);
	}
}