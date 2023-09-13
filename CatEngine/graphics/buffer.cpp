#include "graphics/buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	void buffer::unbind_buffer()
	{
		CAT_GL_SAFE_CHK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void buffer::unbind_vertex_array()
	{
		CAT_GL_SAFE_CHK(glBindVertexArray(0));
	}

	void buffer::unbind_all()
	{
		unbind_buffer();
		unbind_vertex_array();
	}
}