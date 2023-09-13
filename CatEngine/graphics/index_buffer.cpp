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
		CAT_GL_SAFE_CHK(glGenBuffers(1, &m_ebo));
	}

	void index_buffer::bind()
	{

	}

	void index_buffer::clear()
	{
		glDeleteBuffers(1, &m_ebo);

		m_data = nullptr; // reset data pointer
	}
}