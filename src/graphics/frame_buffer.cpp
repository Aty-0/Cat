#include "graphics/frame_buffer.h"
#include "core/utils/logger.h"
#include "graphics/texture.h"

namespace cat::graphics
{
	frame_buffer::frame_buffer() : 
		m_fbo(0), 
		m_rbo(0)
	{
		m_data.push_back(std::make_shared<texture>());
	}

	frame_buffer::~frame_buffer()
	{
		clear();

		m_data[0].reset();
		m_data.clear();
	}

	void frame_buffer::gen()
	{
		glGenFramebuffers(1, &m_fbo);
		bind();

		m_data[0]->createFramebufferTexture();

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
			m_data[0]->getWidth(), m_data[0]->getHeight());

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo); 

		CAT_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		
		unbindBuffer();
	}

	void frame_buffer::unbindBuffer() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void frame_buffer::bind()
	{
		if (m_fbo != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		}
	}

	void frame_buffer::clear()
	{
		if (m_fbo)
		{
			glDeleteFramebuffers(1, &m_fbo);
			glDeleteRenderbuffers(1, &m_rbo);	
			m_data[0]->clear();
		}
	}

	std::shared_ptr<texture> frame_buffer::getTextureShared() const
	{
		return m_data[0];
	}

	texture* frame_buffer::getTexture() const
	{
		return m_data[0].get();
	}
}