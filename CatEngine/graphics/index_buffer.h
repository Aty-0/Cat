#pragma once
#include "graphics/buffer.h"

namespace cat::graphics
{
	class CATENGINE_API index_buffer : public buffer
	{
	public:
		index_buffer();
		~index_buffer() override;

		void gen() override;
		void bind() override;
		void clear() override;
		template<typename type>
		bool setBufferData(std::vector<type> _data, std::uint32_t draw);
	
		void unbindBuffer() override;
		void unbindBufferArray() override;

	private:
		std::uint32_t m_ebo;
	};
}

#include "index_buffer_inline.h"