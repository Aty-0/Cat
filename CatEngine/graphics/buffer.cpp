#include "graphics/buffer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	void buffer::unbindBuffer()
	{

	}

	void buffer::unbindBufferArray()
	{
	
	}

	void buffer::unbindAll()
	{
		unbindBuffer();
		unbindBufferArray();
	}
}