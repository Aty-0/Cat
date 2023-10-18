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
		bool set_buffer_data(std::vector<type> _data, std::uint32_t draw);
	
		void unbind_buffer() override;
		void unbind_buffer_array() override;

	private:
		std::uint32_t m_ebo;
	};
}

#include "index_buffer_inline.h"