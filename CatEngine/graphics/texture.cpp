#include "texture.h"

#include "io/resource_manager.h"
#include "core/game_window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"

namespace cat::graphics
{
	texture::texture() : 
		m_instance(0)
	{

	}

	texture::~texture()
	{
		clear();
	}
	
	void texture::clear()
	{
		if (m_instance != 0)
		{
			glDeleteTextures(1, &m_instance);			
		}
	}

	void texture::create_framebuffer_texture()
	{
		const auto window = core::game_window::get_instance();
		m_width	= window->get_width();
		m_height = window->get_height();
		m_tex_type = GL_TEXTURE_2D;

		glGenTextures(1, &m_instance);
		glBindTexture(GL_TEXTURE_2D, m_instance);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width,
			m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		set_texture_filter(texture::filter::Linear, texture::filter::Linear);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_instance, 0);
		VERB("texture::create_framebuffer_texture %i %i %i", m_instance, m_width, m_height);
	}

	bool texture::load(const char* name)
	{		
		// All stbi supported file extensions except gif
		static const std::vector<const char*> ext = { "jpg", "png", "tga", "bmp", "hdr", "pic", "pnm" };
		std::int32_t width = 0;
		std::int32_t height = 0;
		
		stbi_uc* data = io::resource_manager::get_instance()->read<stbi_uc*, texture>(name, ext); 
		const auto size = io::resource_manager::get_instance()->size<texture>(name, ext) * 4;
		
		if (data == nullptr)
		{
			return false;
		}

		void* stbi_data = static_cast<void*>(stbi_load_from_memory(data, size, &width, &height, &m_nrChannels, 4));
		
		// Other way for load texture 
		//const auto file_path = io::resource_manager::get_instance()->file_path<texture>(name, ext);
		//void* stbi_data = static_cast<void*>(stbi_load(file_path.c_str(), &width, &height, &m_nrChannels, 4));
		
		if (stbi_data == nullptr)
		{
			return false;
		}

		create(GL_TEXTURE_2D, GL_RGBA, width, height, stbi_data);
		
		return true;
	}

	void texture::set_texture_filter(texture::filter filter_min, texture::filter filter_mag)
	{
		m_filter_min = filter_min;
		m_filter_mag = filter_mag;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter_mag);
	}
	
	void texture::set_texture_wrap(texture::wrap wrap)
	{
		m_wrap = wrap;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap);
	}

	void texture::create(std::uint32_t tex_type, std::uint32_t format, std::uint32_t width, std::uint32_t height, void* data)
	{
		VERB("texture::create %i %i %i %i", tex_type, format, width, height);

		m_width  = width;
		m_height = height;
		m_tex_type = tex_type;

		glGenTextures(1, &m_instance);

		glBindTexture(m_tex_type, m_instance);

		set_texture_wrap(texture::wrap::Repeat);
		set_texture_filter(texture::filter::Linear,
			texture::filter::Linear);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(m_tex_type, 0, GL_RGBA, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	void texture::unbind(std::uint32_t active_texture)
	{
		glActiveTexture(active_texture);
		glBindTexture(m_tex_type, 0);
	}

	void texture::bind(std::uint32_t active_texture)
	{
		glActiveTexture(active_texture);
		glBindTexture(m_tex_type, m_instance);
	}

	std::int32_t texture::get_width() const
	{
		return m_width;
	}

	std::int32_t texture::get_height() const
	{
		return m_height;
	}

	std::int32_t texture::get_nrChannels() const
	{
		return m_nrChannels;
	}

	std::uint32_t texture::get_instance() const
	{
		return m_instance;
	}

	std::uint32_t texture::get_tex_type() const
	{
		return m_tex_type;
	}

	texture::filter texture::get_filter_min() const
	{
		return m_filter_min;
	}

	texture::filter texture::get_filter_mag() const
	{
		return m_filter_mag;
	}

	texture::wrap   texture::get_wrap() const
	{
		return m_wrap;
	}
}