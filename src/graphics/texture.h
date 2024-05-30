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
		void setFilter(texture::filter filter_min, texture::filter filter_mag);
		void setWrap(texture::wrap wrap);
		void createFramebufferTexture();

		void bind(std::uint32_t active_texture = GL_TEXTURE0);
		void unbind(std::uint32_t active_texture = GL_TEXTURE0);

		void clear();

		[[nodiscard]] inline std::int32_t   getWidth() const;
		[[nodiscard]] inline std::int32_t   getHeight() const;
		[[nodiscard]] inline std::int32_t   getNrChannels() const;
		[[nodiscard]] inline std::uint32_t  getInstance() const;
		[[nodiscard]] inline std::uint32_t  getType() const;
		[[nodiscard]] inline texture::filter getFilterMin() const;
		[[nodiscard]] inline texture::filter getFilterMag() const;
		[[nodiscard]] inline texture::wrap   getWrap() const;
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