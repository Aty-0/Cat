#pragma once
#include "graphics/buffer.h"

namespace cat::graphics
{
	class CATENGINE_API vertex_buffer : public buffer
	{
	public:
		vertex_buffer();
		~vertex_buffer() override;

		void gen() override;
		void bind() override;
		void clear() override;
		void set_attrib(std::uint32_t num, std::uint32_t size,
			GLenum type, std::int32_t typesize, const void* offset);

		template<typename type>
		bool set_buffer_data(std::vector<type> _data, std::uint32_t draw);

	private:
		std::uint32_t m_vbo;
		std::uint32_t m_vao;


	};
}

#include "vertex_buffer_inline.h"