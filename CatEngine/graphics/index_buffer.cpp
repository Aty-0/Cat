#include "graphics/index_buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	index_buffer::index_buffer()
	{

	}

	index_buffer::~index_buffer()
	{

	}

	void index_buffer::gen()
	{
		glGenBuffers(1, &m_ebo);
	}

	void index_buffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	}

	void index_buffer::clear()
	{
		if (m_ebo)
		{
			glDeleteBuffers(1, &m_ebo);
			m_data = nullptr; // reset data pointer
		}
	}

	void index_buffer::unbind_buffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void index_buffer::unbind_buffer_array()
	{
		glBindVertexArray(0);
	}
}