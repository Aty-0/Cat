#pragma once
#include "core/common.h"

namespace cat::graphics
{
	enum texture_wrap
	{
		Clamp = GL_CLAMP,
		Repeat = GL_REPEAT,
	};

	enum texture_filter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		Nearest_MipMap_Nearest = GL_NEAREST_MIPMAP_NEAREST,
		Linear_MipMap_Nearest = GL_LINEAR_MIPMAP_NEAREST,
		Nearest_MipMap_Linear = GL_NEAREST_MIPMAP_LINEAR,
		Linear_MipMap_Linear = GL_LINEAR_MIPMAP_LINEAR,
	};

	// TODO: Create cubemap, font texture
	class CATENGINE_API texture
	{
	public:
		using wrap   = texture_wrap;
		using filter = texture_filter;

		texture();
		~texture();

		bool load(const char* name);
		void create(std::uint32_t tex_type, std::uint32_t format, std::uint32_t width, std::uint32_t height, void* data);
		void set_texture_filter(texture::filter filter_min, texture::filter filter_mag);
		void set_texture_wrap(texture::wrap wrap);
		void create_framebuffer_texture();

		void bind(std::uint32_t active_texture = GL_TEXTURE0);
		void unbind(std::uint32_t active_texture = GL_TEXTURE0);

		void clear();

		[[nodiscard]] inline std::int32_t   get_width() const;
		[[nodiscard]] inline std::int32_t   get_height() const;
		[[nodiscard]] inline std::int32_t   get_nrChannels() const;
		[[nodiscard]] inline std::uint32_t  get_instance() const;
		[[nodiscard]] inline std::uint32_t  get_tex_type() const;
		[[nodiscard]] inline texture::filter get_filter_min() const;
		[[nodiscard]] inline texture::filter get_filter_mag() const;
		[[nodiscard]] inline texture::wrap   get_wrap() const;
	private:
		std::int32_t  m_width;
		std::int32_t  m_height;
		std::int32_t  m_nrChannels;
		std::uint32_t m_instance;
		std::uint32_t m_tex_type;

		texture::filter m_filter_min;
		texture::filter m_filter_mag;
		texture::wrap   m_wrap;
	};
}