#include "graphics/index_buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	index_buffer::index_buffer()
	{

	}

	index_buffer::~index_buffer()
	{
		clear();
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
			m_data.clear();
			m_data.shrink_to_fit();
		}
	}

	void index_buffer::unbindBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void index_buffer::unbindBufferArray()
	{
		glBindVertexArray(0);
	}

	void index_buffer::setBufferData(const std::vector<std::uint32_t>& data, std::uint32_t draw)
	{
		m_data = data;
		CAT_ASSERT(!m_data.empty());
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size() * sizeof(std::uint32_t), &data[0], GL_STATIC_DRAW);
	}
}