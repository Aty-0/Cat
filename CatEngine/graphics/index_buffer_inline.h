#pragma once
#include "graphics/index_buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	template<typename type>
	inline bool index_buffer::setBufferData(std::vector<type> _data, std::uint32_t draw)
	{
		m_data = _data.data();
		m_size = _data.size();

		if (m_data == nullptr)
		{
			ERR("Index buffer -> data is nullptr");
			return false;
		}

		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size() * sizeof(type), &_data[0], GL_STATIC_DRAW);

		return true;
	}
}