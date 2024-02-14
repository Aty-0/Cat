#pragma once
#include "graphics/buffer.h"

namespace cat::graphics
{
	class texture;
	class CATENGINE_API frame_buffer : public buffer
	{
	public:
		frame_buffer();
		~frame_buffer() override;

		void gen() override;
		void bind() override;
		void clear() override;

		[[nodiscard]] inline texture* getTexture() const;
		[[nodiscard]] inline std::shared_ptr<texture> getTextureShared() const;

		void unbind_buffer() override;
	private:
		std::uint32_t m_fbo;
		std::uint32_t m_rbo;
		std::shared_ptr<texture> m_frameTexture;
	};
}
