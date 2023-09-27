#include "texture.h"

#include "io/resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"

namespace cat::graphics
{
	texture::texture()
	{

	}

	texture::~texture()
	{
		
	}


	bool texture::load(const char* name)
	{		
		static const std::vector<const char*> ext = { "jpg", "png", "tga", "bmp", "hdr", "pic", "pnm" };
		
		// All stbi supported file extensions except gif
		stbi_uc* data = io::resource_manager::get_instance()->read<stbi_uc*, texture>(name, ext); 
		const auto size = io::resource_manager::get_instance()->size<texture>(name, ext);

		if (data == nullptr)
		{
			return false;
		}
		std::int32_t width = 0;
		std::int32_t height = 0;
		void* stbi_data = static_cast<void*>(stbi_load_from_memory(data, size, &width, &height, &m_nrChannels, 0));
		
		if (stbi_data == nullptr)
		{
			return false;
		}

		create(GL_TEXTURE_2D, GL_RGBA, width, height, stbi_data);
		
		return true;
	}

	void texture::set_texture_filter(texture_filter filter_min, texture_filter filter_mag)
	{
		m_filter_min = filter_min;
		m_filter_mag = filter_mag;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter_mag);
	}
	
	void texture::set_texture_wrap(texture_wrap wrap)
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

		set_texture_wrap(texture_wrap::Repeat);
		set_texture_filter(texture_filter::Linear_MipMap_Linear, texture_filter::Linear);

		glTexImage2D(m_tex_type, 0, GL_RGB, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
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

	texture::texture_filter texture::get_filter_min() const
	{
		return m_filter_min;
	}

	texture::texture_filter texture::get_filter_mag() const
	{
		return m_filter_mag;
	}

	texture::texture_wrap   texture::get_wrap() const
	{
		return m_wrap;
	}
}