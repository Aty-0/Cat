#pragma once
#include "graphics/buffer.h"

namespace cat::graphics
{
	struct vertex;
	class CATENGINE_API vertex_buffer : public buffer<graphics::vertex>
	{
	public:
		vertex_buffer();
		~vertex_buffer() override;

		void gen() override;
		void bind() override;
		void clear() override;
		void setAttrib(std::uint32_t num, std::uint32_t size,
			GLenum type, std::int32_t typesize, const void* offset);
		void setBufferData(const std::vector<graphics::vertex>& _data, std::uint32_t draw);
		void unbindBuffer() override;
		void unbindBufferArray() override;

	private:
		std::uint32_t m_vbo;
		std::uint32_t m_vao;


	};
}
