#include "graphics/buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	void buffer::unbind_buffer()
	{

	}

	void buffer::unbind_buffer_array()
	{
	
	}

	void buffer::unbind_all()
	{
		unbind_buffer();
		unbind_buffer_array();
	}
}