#pragma once
#include "graphics/buffer.h"

namespace cat::graphics
{
	class CATENGINE_API index_buffer : public buffer<std::uint32_t>
	{
	public:
		index_buffer();
		~index_buffer() override;

		void gen() override;
		void bind() override;
		void clear() override;
		void setBufferData(const std::vector<std::uint32_t>& data, std::uint32_t draw);
	
		void unbindBuffer() override;
		void unbindBufferArray() override;

	private:
		std::uint32_t m_ebo;
	};
}
