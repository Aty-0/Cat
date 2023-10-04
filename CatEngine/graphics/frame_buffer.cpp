#include "graphics/frame_buffer.h"
#include "core/utils/logger.h"
#include "graphics/texture.h"

namespace cat::graphics
{
	frame_buffer::frame_buffer()
	{

	}

	frame_buffer::~frame_buffer()
	{

	}

	void frame_buffer::gen()
	{
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_frame_buff_tex = new texture();
		m_frame_buff_tex->create_framebuffer_texture();

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
			m_frame_buff_tex->get_width(), m_frame_buff_tex->get_height()); 

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo); 

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ERR("Framebuffer is failed");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void frame_buffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void frame_buffer::clear()
	{
		glDeleteBuffers(1, &m_fbo);
	}

	texture* frame_buffer::get_texture() const
	{
		return m_frame_buff_tex;
	}
}