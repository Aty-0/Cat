#pragma once
#include "graphics/vertex_buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	template<typename type>
	inline bool vertex_buffer::set_buffer_data(std::vector<type> _data, std::uint32_t draw)
	{
		m_data = _data.data();

		if (m_data == nullptr)
		{
			ERR("Index buffer -> data is nullptr");
			return false;
		}

		CAT_GL_SAFE_CHK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
		// set buffer data 
		CAT_GL_SAFE_CHK(glBufferData(GL_ARRAY_BUFFER, size() * sizeof(type), &_data[0], GL_STATIC_DRAW));

		return true;
	}
}