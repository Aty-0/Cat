#include "graphics/piece.h"
#include "graphics/renderer.h"
#include "core/utils/logger.h"

namespace cat::graphics
{
	piece::piece()
	{
		std::vector<graphics::vertex> vertices = { { glm::vec3(0.5f,  1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, -1.0f) },      // top right
										{ glm::vec3(0.5f, -1.0f, 0.0f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },      // bottom right
										{ glm::vec3(-1.0f, -1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, 0.0f) },      // bottom left
										{ glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(-1.0f, -1.0f) }       // top left 
		};

		std::vector<std::uint32_t> indices =
		{
			0, 1, 3,  // first triangle
			1, 2, 3   // second triangle
		};
		
		createPiece(vertices, indices, { "default_texture" }, "piece");
	}

	piece::piece(
		const std::vector<graphics::vertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<const char*>& texture_names,
		const char* shader_name
		)
	{
		createPiece(vertices, indices, texture_names, shader_name);
	}

	void piece::createPiece(const std::vector<graphics::vertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<const char*>& texture_names,
		const char* shader_name)
	{
		m_vertex_buffer = std::make_shared<graphics::vertex_buffer>(*new graphics::vertex_buffer());

		m_vertex_buffer->gen();
		m_vertex_buffer->set_buffer_data<graphics::vertex>(vertices, GL_STATIC_DRAW);

		m_index_buffer = std::make_shared<graphics::index_buffer>(*new graphics::index_buffer());
		m_index_buffer->gen();
		m_index_buffer->set_buffer_data<std::uint32_t>(indices, GL_STATIC_DRAW);

		m_vertex_buffer->set_attrib(0, 3, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, pos)));
		m_vertex_buffer->set_attrib(1, 4, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, color)));
		m_vertex_buffer->set_attrib(2, 2, GL_FLOAT, sizeof(graphics::vertex), reinterpret_cast<void*>(offsetof(graphics::vertex, uv)));

		m_vertex_buffer->unbind_buffer();

		m_shader = std::make_shared<graphics::shader>(*new graphics::shader());
		CAT_ASSERT(m_shader->load(shader_name));

		for (auto name : texture_names)
		{
			const auto texture = std::make_shared<graphics::texture>(*new graphics::texture());
			CAT_ASSERT(texture->load(name));

			m_textures.push_back(texture);
		}
	}

	piece::~piece()
	{
		m_shader.reset();

		for (auto texture : m_textures)
			texture.reset();

		m_vertex_buffer->clear();
		m_vertex_buffer.reset();
		m_index_buffer->clear();
		m_index_buffer.reset();
	}

	void piece::reloadTextures()
	{
		// TODO:
	}

	void piece::begin()
	{
		m_shader->bind();
	}

	void piece::end(graphics::renderer* render)
	{
		std::uint32_t index_bind = 0;
		for (auto texture : m_textures)
		{
			texture->bind(GL_TEXTURE0 + index_bind);
			index_bind++;
		}

		m_vertex_buffer->bind();
		const auto size = m_index_buffer->size();
		render->draw_elements(size, GL_TRIANGLES);

		std::uint32_t index_unbind = 0;
		for (auto texture : m_textures)
		{
			texture->unbind(GL_TEXTURE0 + index_unbind);
			index_unbind++;
		}
	}
	graphics::shader* piece::getShader() const
	{
		return m_shader.get();
	}
	
	graphics::vertex_buffer* piece::getVertexBuffer()  const
	{
		return m_vertex_buffer.get();
	}
	
	graphics::index_buffer* piece::getIndexBuffer()  const
	{
		return m_index_buffer.get();
	}

}